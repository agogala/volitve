# $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
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
