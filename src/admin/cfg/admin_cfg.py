# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: admin_cfg.py 1.2.2.7 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
#
# Konfiguracija za administracijo:

# Direktoriji:
ROOTDIR = "/home/andrej/sola/volitve/src"
tempdir = ROOTDIR + "/templates"
htmldir = ROOTDIR + "/HTML"
userurl = "/uporabniki"
userdir = htmldir + userurl

# Obrazci:
templates = {"Dodaj": {'dir': '/narocila', 'ime': 'dodaj.html'},
	     "Pregled": {'dir': '/narocila', 'ime': 'pregled.html'},
	     "RegisterNew": {'ime': 'register-new.txt'},
	     "RegisterOld": {'ime': 'register-old.txt'},
	     "NoRegistration": {'ime': 'no-registration.txt'},
	     "HTAccess": {'dir': '/uporabniki', 'ime': 'htaccess'},
	     "Stanje": {'dir': '/uporabniki', 'ime': 'stanje.html'},
	     "Tecaj": {'dir': '/pregled/tecaji', 'ime': 'tecaj.html'},
	     "IndexTecaji": {'dir': '/pregled', 'ime': 'tecaji.html'},
	     "Pogodbe": {'dir': '/uporabniki', 'ime': 'pogodbe.html'}}

# Baza:
DB_Name = "volitve_d"

# Sendmail
SENDMAIL = "/usr/lib/sendmail -t -em"

# Porti:
Notify_Port = 5002

# Password file:
PasswdFile = "/home/andrej/sola/volitve/src/admin/cfg/htpasswd"

def ReadCfg():
    return

# Trgovanje teèe, ali je ustavljeno:
_ClosingTime = 880153200 # 'Sat Nov 22 00:00:00 1997' :-)))
#_ClosingTime = 880117058 # 'Sat Nov 22 00:00:00 1997' :-)))

def MarketClosed():
    import time
    return time.time()>=_ClosingTime
