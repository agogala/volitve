# $ProjectHeader: volitve 0.12 Mon, 22 Sep 1997 15:21:03 +0200 andrej $
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
