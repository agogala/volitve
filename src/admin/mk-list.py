#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: mk-list.py 1.1 Sat, 27 Dec 1997 15:06:49 +0000 andrej $
# Izpi¹i poravnave

import os, sys

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import MakeList
    MakeList.run()

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
