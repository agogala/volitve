# $ProjectHeader: volitve 0.14 Thu, 25 Sep 1997 21:32:05 +0200 andrej $
#
# $Id: initfmt.py 1.2 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
#
# Inicializira formatiranje

import os, sys

try:
    exename = "initfmt.py"
    admindir = '../admin'
    sys.path = [os.path.join(admindir, 'lib'), \
                os.path.join(admindir, 'cfg')] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import MakePregled

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
