# $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
#
# $Id: admin_cfg.py 1.2.2.6 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
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
