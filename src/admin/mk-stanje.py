#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: mk-stanje.py 1.1 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
# Popravi stanje uporabnika.

import os, sys

print "MAKE STANJE"

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path

    if len(sys.argv)<>2:
	print "Usage: %s <username>" % exename
	sys.exit(1)
    user = sys.argv[1]

    import admin_cfg
    admin_cfg.ReadCfg()
    import MakePregled
    userroot = admin_cfg.htmldir + \
	       admin_cfg.templates['Stanje']['dir'] 

    MakePregled.updateuser(admin_cfg.tempdir, userroot,
			   admin_cfg.templates, user)

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
