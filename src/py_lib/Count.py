# $ProjectHeader: volitve 0.21 Tue, 21 Oct 1997 21:26:41 +0200 andrej $
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
