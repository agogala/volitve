# $ProjectHeader: volitve 0.17 Fri, 03 Oct 1997 17:45:58 +0200 andrej $
#
# $Id: DBConn.py 1.1 Fri, 26 Sep 1997 16:28:00 +0000 andrej $
#
# Vzpostavi zvezo z bazo

import pg95
import admin_cfg

pg95.set_defbase(admin_cfg.DB_Name)
db_conn = pg95.connect()
