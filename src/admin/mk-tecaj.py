#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.26 Sat, 08 Nov 1997 08:02:11 +0100 andrej $
#
# $Id: mk-tecaj.py 1.2 Mon, 03 Nov 1997 13:25:50 +0000 andrej $
# Naredi tecaj.html

import os, sys

print "MAKE TECAJ"

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()

    if len(sys.argv)==2:
	dan = sys.argv[1]
    else:
	dan = 'yesterday'

    import MakeTecaj
    MakeTecaj.run(admin_cfg.tempdir, admin_cfg.htmldir, 
		  admin_cfg.templates, dan)

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
