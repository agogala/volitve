# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: initfmt.py 1.3 Sat, 08 Nov 1997 07:02:11 +0000 andrej $
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
    import MakePogodbe

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
