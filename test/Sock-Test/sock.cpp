#define ACE_NTRACE 1

#include <ace/INET_Addr.h>
#include <ace/SOCK_Dgram.h>

//#include <iostream.h>

struct ACE_Bcast_Node 
{
  ACE_Bcast_Node (ACE_INET_Addr &, ACE_Bcast_Node *);

  ACE_INET_Addr bcast_addr_;
  // Broadcast address for the interface.

  ACE_Bcast_Node *next_;
  // Pointer to the next interface in the chain.
};

class ACE_Export ACE_SOCK_Dgram_Bcast : public ACE_SOCK_Dgram
  // = TITLE
  //     Defines the member functions for the ACE_SOCK datagram
  //     abstraction. 
{
public:
  // = Initialization and termination methods.
  ACE_SOCK_Dgram_Bcast (void);
  // Default constructor.

  int open (const ACE_Addr &local, 
            int protocol_family = PF_INET, 
            int protocol = 0,
            int reuse_addr = 0,
            const char *host_name = 0);
  // Initiate a connectionless datagram broadcast endpoint.

private:
  int mk_broadcast (const char *host_name);
  // Make broadcast available for Datagram socket.

  ACE_Bcast_Node *if_list_;
  // Points to the head of the list of broadcast interfaces.

};


ACE_Bcast_Node::ACE_Bcast_Node (ACE_INET_Addr &addr,
				ACE_Bcast_Node *next)
  : bcast_addr_ (addr), 
    next_ (next)
{
  ACE_TRACE ("ACE_Bcast_Node::ACE_Bcast_Node");
}


// Here's the simple-minded constructor.
ACE_SOCK_Dgram_Bcast::ACE_SOCK_Dgram_Bcast (void)
  : if_list_ (0)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Bcast::ACE_SOCK_Dgram_Bcast");
}

// Here's the general-purpose open routine.

int
ACE_SOCK_Dgram_Bcast::open (const ACE_Addr &local, 
			    int protocol_family, 
			    int protocol,
			    int reuse_addr,
                            const char *host_name)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Bcast::open");
  if (this->ACE_SOCK_Dgram::open (local, protocol_family, 
				  protocol, reuse_addr) == -1)
    return -1;

  return this->mk_broadcast (host_name);
}

// Make broadcast available for Datagram socket.

int
ACE_SOCK_Dgram_Bcast::mk_broadcast (const char *host_name)
{
  ACE_TRACE ("ACE_SOCK_Dgram_Bcast::mk_broadcast");

  int one = 1;

  if (ACE_OS::setsockopt (this->get_handle (), SOL_SOCKET, SO_BROADCAST, 
			  (char *) &one, sizeof one) == -1)
    return -1;

#if !defined(ACE_WIN32)
  ACE_HANDLE s = this->get_handle ();

  char buf[BUFSIZ];
  struct ifconf ifc;

  ifc.ifc_len = sizeof buf;
  ifc.ifc_buf = buf;

  // Get interface structure and initialize the addresses using UNIX
  // techniques.
  if (ACE_OS::ioctl (s, SIOCGIFCONF, (char *) &ifc) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", 
		      "ACE_SOCK_Dgram_Bcast::mk_broadcast: ioctl (get interface configuration)"),
		      ACE_INVALID_HANDLE);

  struct ifreq *ifr = ifc.ifc_req;

  struct sockaddr_in host_addr;

  //Get host ip address
  if (host_name)
    {
      hostent *hp = ACE_OS::gethostbyname (host_name);

      if (hp == 0)
	return -1;
      else
	ACE_OS::memcpy ((char *) &host_addr.sin_addr.s_addr, 
			(char *) hp->h_addr, 
			hp->h_length);
    }

  for (int n = ifc.ifc_len / sizeof (struct ifreq) ; n > 0; n--, ifr++) 
    {
      // Compare host ip address with interface ip address.
      if (host_name)
        {
	  struct sockaddr_in if_addr;

          ACE_OS::memcpy (&if_addr, &ifr->ifr_addr, sizeof if_addr);

          if (host_addr.sin_addr.s_addr != if_addr.sin_addr.s_addr)
	    continue;
        }

      if (ifr->ifr_addr.sa_family != AF_INET) 
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", 
		      "ACE_SOCK_Dgram_Bcast::mk_broadcast: Not AF_INET"));
	  continue;
	}

      struct ifreq flags = *ifr;
      struct ifreq if_req = *ifr;

      if (ACE_OS::ioctl (s, SIOCGIFFLAGS, (char *) &flags) == -1)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", 
		     "ACE_SOCK_Dgram_Bcast::mk_broadcast: ioctl (get interface flags)"));
	  continue;
	}   

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_UP) == 0)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", 
		     "ACE_SOCK_Dgram_Bcast::mk_broadcast: Network interface is not up"));
	  continue;
	}

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_LOOPBACK))
	continue;

      if (ACE_BIT_ENABLED (flags.ifr_flags, IFF_BROADCAST))
	{
	  if (ACE_OS::ioctl (s, SIOCGIFBRDADDR, (char *) &if_req) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n", 
		       "ACE_SOCK_Dgram_Bcast::mk_broadcast: ioctl (get broadaddr)"));
	  else 
	    {
	      ACE_INET_Addr addr ((sockaddr_in *) &if_req.ifr_broadaddr,
				  sizeof if_req.ifr_broadaddr);
	      ACE_NEW_RETURN (this->if_list_, ACE_Bcast_Node (addr, this->if_list_), -1);
	    }
	}
      else 
	ACE_ERROR ((LM_ERROR, "%p\n", 
		   "ACE_SOCK_Dgram_Bcast::mk_broadcast: Broadcast is not enable for this interface."));
    }
#else
  ACE_UNUSED_ARG (host_name);

  ACE_INET_Addr addr (u_short (0), ACE_UINT32 (INADDR_BROADCAST));
  ACE_NEW_RETURN (this->if_list_, ACE_Bcast_Node (addr, this->if_list_), -1);
#endif /* !ACE_WIN32 */
  return this->if_list_ == 0 ? -1 : 0;
}


int
main() {

  ACE_SOCK_Dgram_Bcast sock;

  ACE_DEBUG((LM_DEBUG, "Ace_ntrace: %d\n", ACE_NTRACE));

  ACE_DEBUG((LM_DEBUG, "Opening socket\n"));
  if (sock.open(ACE_INET_Addr(50000))==-1) 
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else
    ACE_DEBUG((LM_DEBUG, "Socket opened\n"));


  return 0;
}
