# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: MakePoravnave.py 1.1 Sat, 27 Dec 1997 15:06:49 +0000 andrej $
#
# Izraèunaj poravnave.

import pg95
import admin_cfg
import Util
import DBConn
import string

conn = DBConn.db_conn

def run():
    
