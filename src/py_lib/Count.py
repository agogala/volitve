# $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
#
# $Id: Count.py 1.1 Fri, 29 Aug 1997 11:44:29 +0000 andrej $
#
# ©teje zahtevke:

cnt = 0

def increase():
    global cnt
    cnt = cnt + 1

def value():
    global cnt
    return cnt
