#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
#
# $Id: mk-pregled.py 1.3 Mon, 03 Nov 1997 13:25:50 +0000 andrej $
# Naredi pregled.html

import os, sys

print "MAKE PREGLED"

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import MakePregled
    MakePregled.run(admin_cfg.tempdir, admin_cfg.htmldir, 
		    admin_cfg.templates)

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
