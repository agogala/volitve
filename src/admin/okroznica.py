#!/net/rozle/export/share/sol/bin/python

# Po¹lji okro¾nico

import os, sys

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path

    import admin_cfg
    admin_cfg.ReadCfg()
    import Okroznica
    Okroznica.run()

except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback

