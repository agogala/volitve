# $ProjectHeader: volitve 0.24 Mon, 03 Nov 1997 14:25:50 +0100 andrej $
#
# $Id: FormatResponse.py 1.3 Thu, 09 Oct 1997 13:19:34 +0000 andrej $
#
# Oblikuje cgi odgovor.

def FormatResponse(str, title="", headers={}):
    body = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2//EN">\n' + \
	   '<html><head>\n' + \
	   '<meta http-equiv="Content-type" content="text/html; charset=iso-8859-2">\n' + \
	   '<title>%s</title>\n' % title + \
	   '</head><body bgcolor="#ffe8b8"><hr>\n' +  \
	   str + \
	   "</body>\n"
    
    print "Content-type: text/html"
    for h in headers.keys():
	print "%s: %s" % (h, headers[h])
    print "Content-length: %d" % len(body)
    print 
    print body

    
