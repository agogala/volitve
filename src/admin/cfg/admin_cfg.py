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
