# $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
#
# $Id: Okroznica.py 1.1 Tue, 04 Nov 1997 18:56:32 +0000 andrej $
#
# Po¹lji okro¾nico vsem

import DBConn
import sys
import os
import admin_cfg

# Imamo kar stalno povezavo z bazo:
conn = DBConn.db_conn

def run():
    msg = sys.stdin.read()
    
    email = conn.query("SELECT email FROM stranke Where stranka_id='andrej'")

    for u in email:
	message = msg % u[0]
	pipe = os.popen(admin_cfg.SENDMAIL, "w")
	pipe.write(message)
	pipe.close()
	
    
