# $ProjectHeader: volitve 0.18 Sun, 05 Oct 1997 22:53:12 +0200 andrej $
#
# $Id: FormatResponse.py 1.2 Wed, 24 Sep 1997 17:03:46 +0000 andrej $
#
# Oblikuje cgi odgovor.

def FormatResponse(str, title=""):
    print "Content-type: text/html; charset=iso-8859-2"
    print 
    
    print '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">'
    print "<html><head>\n"
    print '<meta http-equiv="Content-type" content="text/html; charset=iso-8859-2">\n'
    print '<title>%s</title>\n' % title 
    print '</head><body bgcolor="#ffe8b8"><hr>\n'
    print str
    print "</body>\n"
