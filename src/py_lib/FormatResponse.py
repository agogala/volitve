# $ProjectHeader: volitve 0.11 Thu, 11 Sep 1997 23:18:12 +0200 andrej $
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
