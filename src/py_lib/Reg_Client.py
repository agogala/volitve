# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: Reg_Client.py 1.6 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
#
# Registriraj uporabnika:

import os, cgi, string
import Admin_Client, FormatResponse, CheckForm
import cgi_config
import time

# Vrne obrazec:
def GetForm(hash):
    dict = {'Hash': hash}
    f = open(os.path.join(cgi_config.tempdir, 'reg_form.html.in'))
    content = f.read()
    f.close()
    return content % dict

def HandleHash(form):
    fields = ["hash"]
    reqfields = fields
    if not CheckForm.CheckForm(form, fields, reqfields):
	return FormatResponse.FormatResponse('<H1>Napaka</H1>\n' + \
					     'Prosimo izpolnite vsa polja...')
    try:
	hash = form["hash"].value
    except:
	return FormatResponse.FormatResponse('<H1>Napaka</H1>\n' + \
					     'Prosimo izpolnite vsa polja...')

    # Preveri Hash:
    #    [...]
    Admin_Client.admin_client.send("Validate %s" % hash)
    resp = string.atoi(Admin_Client.admin_client.response())
    if resp==0:	
	return FormatResponse.FormatResponse(GetForm(hash), "Registracija",
					     {'Pragma': 'no-cache'})
    elif resp==100:
	# No such hash:
	return FormatResponse.FormatResponse("<H1>Napaka</H1>\n" +
					    "Nepravilen kljuè")
    elif resp==101:
	# Hash accessed:
	return FormatResponse.FormatResponse("<H1>Napaka</H1>\n" +
					    "Nepravilen kljuè")
    elif resp==300:
	# Zaprt trg:
	return FormatResponse.FormatResponse("<H1>Zaprta borza</H1>\n" +
					     "Borza je zaprta")
    else:
	return FormatResponse.FormatResponse("<H1>Napaka</H1>\n" +
					    "Interna napaka")

rel_msg = \
"Status: 302 relocate\n" \
"Location: /narocila\n" \
"Content-Type: text/plain\n" \
"\n" \
"Document relocated\n" \

# Registrira uporabnika:
def HandleRegister(form):
    fields = ["hash", "username", "passwd", "passwd1"]
    reqfields = fields
    try:
	hash = form['hash'].value
    except:
	# Kako obravnavati tale primer?
	hash = ""
    if not CheckForm.CheckForm(form, fields, reqfields):
	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>\n"  + \
	    "Prosim izpolnite vsa polja..." +
	    GetForm(hash), "Registracija")
    
    if form['passwd'].value<>form['passwd1'].value:
	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>\n" +
	    "Geslo razlièno vtipkano" +
	    GetForm(hash), "Registracija")

    # Registriraj:
    username = form['username'].value
    passwd = form['passwd'].value
    Admin_Client.admin_client.send('Register %s "%s" "%s"' % (hash, username, passwd))
    resp = string.atoi(Admin_Client.admin_client.response())
    if resp==0:
	# redirect:
	print rel_msg
	return
    elif resp==100:
	# No such hash 
 	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>" +
	    "Napaèen obrazec")
    elif resp==101:
	# Duplicate e-mail
 	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>" +
	    "Uporabnik z enakim elektronskim naslovom je ¾e prijavljen")
    elif resp==102:
	# Duplicate user-name:
	return FormatResponse.FormatResponse(
	    "<H1>Napaka</H1>" + 
	    'Uporabni¹ko ime "%s" je ¾e uporabljeno. Prosim, izberite drugaènega' +
	    GetForm(hash), "Registracija")
    else:
	# Internal error:
	return FormatResponse.FormatResponse("<H1>Napaka</H1>\n" +
					    "Interna napaka")

# Se odloèa:
def HandleForm(form):
    if Admin_Client.admin_client.open()<0:
	return FormatResponse.FormatResponse("<H1>Napaka</H1>\n" + \
	       "Administrator je zaprt\n")

    if form.has_key('op'):
	if string.upper(form['op'].value)=='HASH':
	    return HandleHash(form)
	else:
	    return HandleRegister(form)
    else:
	return FormatResponse.FormatResponse("<H1>Napaka</H1>\n" + 
					     "Prosimo izpolnite vsa polja...")
