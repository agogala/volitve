# $ProjectHeader: volitve 0.21 Tue, 21 Oct 1997 21:26:41 +0200 andrej $
#
# $Id: Util.py 1.2 Tue, 21 Oct 1997 19:26:41 +0000 andrej $
#
# Razlièni podprogrami

import fcntl, FCNTL
import os

def MakeTemplate(templname, destname, dict):
    # Preberi obrazec:
    f = open(templname, "r")
    content = f.read()
    f.close()
    g = open(destname, "w")
#   Zaklepanje: upam, da ne bo povzroèilo te¾av...
    os.chmod(destname, 02644)
    fcntl.flock(g.fileno(), FCNTL.F_WRLCK)    
    g.write(content % dict)
    g.close


def RewriteDate(d):
    return '%s.%s.%s' % (d[3:5], d[0:2], d[8:10])
