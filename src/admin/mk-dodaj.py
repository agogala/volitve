#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: mk-dodaj.py 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
# Naredi dodaj.html
import os, sys

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import MakeDodaj
    MakeDodaj.run(admin_cfg.tempdir, admin_cfg.htmldir, admin_cfg.templates)

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
