#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: mk-pogodbe.py 1.1 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
# Popravi stanje uporabnika.

import os, sys

print "MAKE POGODBE"

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path

    if len(sys.argv)<>3:
	print "Usage: %s <username> <papir_id>" % exename
	sys.exit(1)
    user = sys.argv[1]
    paper = sys.argv[2]

    import admin_cfg
    admin_cfg.ReadCfg()
    import MakePogodbe
    userroot = admin_cfg.htmldir + \
	       admin_cfg.templates['Pogodbe']['dir'] 

    MakePogodbe.run(admin_cfg.tempdir, userroot,
		    admin_cfg.templates, user, paper)

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
