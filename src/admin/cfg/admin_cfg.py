# $ProjectHeader: volitve 0.20 Sun, 19 Oct 1997 19:07:54 +0200 andrej $
#
# $Id: admin_cfg.py 1.2.2.3 Sun, 05 Oct 1997 20:53:12 +0000 andrej $
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
	     "Stanje": {'dir': '/uporabniki', 'ime': 'stanje.html'}}

# Baza:
DB_Name = "volitve"

# Sendmail
SENDMAIL = "/usr/lib/sendmail -t -em"

# Porti:
Notify_Port = 5002

# Password file:
PasswdFile = "/home/andrej/sola/volitve/src/admin/cfg/htpasswd"

def ReadCfg():
    return
