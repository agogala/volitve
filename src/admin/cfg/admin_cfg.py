# $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
#
# $Id: admin_cfg.py 1.2.2.1 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
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
	     "Pregled": {'dir': '/pregled', 'ime': 'pregled.html'},
	     "RegisterNew": {'ime': 'register-new.txt'},
	     "RegisterOld": {'ime': 'register-old.txt'}}

# Baza:
DB_Name = "volitve"

# Sendmail
SENDMAIL = "/usr/lib/sendmail -t -em"

# Porti:
Notify_Port = 5002

def ReadCfg():
    return
