#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.23 Tue, 28 Oct 1997 21:15:29 +0100 andrej $
#
# $Id: mk-tecaj.py 1.1 Sun, 19 Oct 1997 17:07:54 +0000 andrej $
# Naredi tecaj.html

import os, sys

print "MAKE TECAJ"

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import MakeTecaj
    MakeTecaj.run(admin_cfg.tempdir, admin_cfg.htmldir, admin_cfg.templates)

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
