# $ProjectHeader: volitve 0.22 Sun, 26 Oct 1997 22:47:33 +0100 andrej $
#
# $Id: DBConn.py 1.1 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
#
# Vzpostavi zvezo z bazo

import pg95
import admin_cfg

pg95.set_defbase(admin_cfg.DB_Name)
db_conn = pg95.connect()
