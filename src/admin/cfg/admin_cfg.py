# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: admin_cfg.py 1.2.2.8 Sat, 27 Dec 1997 15:06:49 +0000 andrej $
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
#DB_Name = "volitve_d"
DB_Name = "zupani"

# Sendmail
SENDMAIL = "/usr/lib/sendmail -t -em"

# Porti:
Notify_Port = 5002

# Password file:
PasswdFile = "/home/andrej/sola/volitve/src/admin/cfg/htpasswd"

def ReadCfg():
    return

# Trgovanje teèe, ali je ustavljeno:
_StartingTime = 881139600 # 'Wed Dec  3 10:00:00 1997'
_ClosingTime = 882572400 # 'Sat Dec 20 00:00:00 1997'

def StartingTime():
    return _StartingTime

def MarketClosed():
    import time
    return (time.time()>=_ClosingTime) or (time.time()<_StartingTime)
