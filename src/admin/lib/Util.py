# $ProjectHeader: volitve 0.27 Fri, 21 Nov 1997 18:06:57 +0100 andrej $
#
# $Id: Util.py 1.4 Tue, 28 Oct 1997 20:15:29 +0000 andrej $
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
    g.close()


def RewriteDate(d):
    return '%s.%s.%s' % (d[3:5], d[0:2], d[8:10])

def si2iso(d):
    return d[6:10] + d[3:5] + d[0:2]

def iso2si(d):
    return d[4:6] + '.' + d[2:4] + '.' + d[0:2]

def ReformatFloat(s):
    r = ''
    for i in s:
	if i=='.':
	    r = r + ','
	elif i==',':
	    r = r + '.'
	else:
	    r = r + i
    return r
    
def FormatFloat(f):
    s = "%.2f" % f
    return ReformatFloat(s)
