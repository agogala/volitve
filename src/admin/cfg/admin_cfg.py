# $ProjectHeader: volitve 0.6 Fri, 05 Sep 1997 16:43:33 +0200 andrej $
#
# $Id: admin_cfg.py 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
#
# Konfiguracija za administracijo:

# Direktoriji:
ROOTDIR = "/home/andrej/sola/volitve/src"
tempdir = ROOTDIR + "/templates"
htmldir = ROOTDIR + "/HTML"

# Obrazci:
templates = {"Dodaj": {'dir': '/narocila', 'ime': 'dodaj.html'},
	     "Pregled": {'dir': '/pregled', 'ime': 'pregled.html'}}

# Baza:
DB_Name = "volitve"

# Porti:
Notify_Port = 5002

def ReadCfg():
    return
