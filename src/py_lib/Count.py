# $ProjectHeader: volitve 0.25 Tue, 04 Nov 1997 19:56:32 +0100 andrej $
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
