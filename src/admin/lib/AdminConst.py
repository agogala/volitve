# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: AdminConst.py 1.2 Fri, 21 Nov 1997 17:06:57 +0000 andrej $
#
# Admin protokol odgovori:

OK=0

# Validate:
class Validate:
    OK             = 000
    NoHash         = 100
    HashAccessed   = 101
    MarketClosed   = 300
    InternalError  = 500

# Register
class Register:
    OK             = 000
    NoHash         = 100
    DuplicateEmail = 101
    DuplicateUser  = 102
    MarketClosed   = 300
    InternalError  = 500

# UserID
class UserID:
    OK             = 000
    NoUser         = 100
    MarketClosed   = 300
    InternalError  = 500
    
