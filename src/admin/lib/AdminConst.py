# $ProjectHeader: volitve 0.22 Sun, 26 Oct 1997 22:47:33 +0100 andrej $
#
# $Id: AdminConst.py 1.1 Mon, 22 Sep 1997 13:21:03 +0000 andrej $
#
# Admin protokol odgovori:

OK=0

# Validate:
class Validate:
    OK             = 000
    NoHash         = 100
    HashAccessed   = 101
    InternalError  = 500

# Register
class Register:
    OK             = 000
    NoHash         = 100
    DuplicateEmail = 101
    DuplicateUser  = 102
    InternalError  = 500

# UserID
class UserID:
    OK             = 000
    NoUser         = 100
    InternalError  = 500
    
