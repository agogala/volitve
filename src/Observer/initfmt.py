# $ProjectHeader: volitve 0.9 Wed, 10 Sep 1997 18:15:50 +0200 andrej $
#
# $Id: initfmt.py 1.1 Mon, 08 Sep 1997 15:37:41 +0000 andrej $
#
# Inicializira formatiranje

import os, sys

try:
    exename = "init.py"
    admindir = '../admin'
    sys.path = [os.path.join(admindir, 'lib'), \
                os.path.join(admindir, 'cfg')] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import MakePregled

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
