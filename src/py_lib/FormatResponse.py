# $ProjectHeader: volitve 0.8 Tue, 09 Sep 1997 00:58:50 -2200 andrej $
#
# $Id: FormatResponse.py 1.1 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
#
# Oblikuje cgi odgovor.

def FormatResponse(str):
    print "Content-type: text/html; charset=iso-8859-2"
    print 
    
    print "<html><head></head><body><hr>\n"
    print str
    print "</body>\n"
