# $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
#
# $Id: initadm.py 1.1 Thu, 11 Sep 1997 16:28:32 +0000 andrej $
#
# Inicializira administratorja

import os, sys

try:
    exename = "initadm.py"
    admindir = '../admin'
    sys.path = [os.path.join(admindir, 'lib'), \
                os.path.join(admindir, 'cfg')] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import Registrator

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
