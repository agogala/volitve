#!/net/rozle/export/share/sol/bin/python
# $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
#
# $Id: reg-mail.py 1.1 Wed, 10 Sep 1997 16:15:50 +0000 andrej $
# Odgovori na zahtevek za registracijo
import os, sys

UserError='UserError'

try:
    (exedir, exename) = os.path.split(sys.argv[0])
    os.chdir(os.path.join(exedir, 'lib'))
    sys.path = ['.', '../cfg'] + sys.path
    import admin_cfg
    admin_cfg.ReadCfg()
    import RegisterReply
    RegisterReply.run(admin_cfg.tempdir, admin_cfg.templates)

except UserError, val:
    print val
    sys.exit(1)
except:
    sys.stderr.write('*** Error running %s ***\n' % exename)
    raise sys.exc_type, sys.exc_value, sys.exc_traceback
