# $ProjectHeader: volitve 0.22 Sun, 26 Oct 1997 22:47:33 +0100 andrej $
#
# $Id: Util.py 1.3 Sun, 26 Oct 1997 21:47:33 +0000 andrej $
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
    s = "%5.2f" % f
    return ReformatFloat(s)
