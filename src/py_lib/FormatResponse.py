# $ProjectHeader: volitve 0.10 Thu, 11 Sep 1997 18:28:32 +0200 andrej $
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
