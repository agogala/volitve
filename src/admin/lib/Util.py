# $ProjectHeader: volitve 0.13 Wed, 24 Sep 1997 19:03:46 +0200 andrej $
#
# $Id: Util.py 1.1 Thu, 11 Sep 1997 21:18:12 +0000 andrej $
#
# Razli�ni podprogrami


def MakeTemplate(templname, destname, dict):
    # Preberi obrazec:
    f = open(templname, "r")
    content = f.read()
    f.close();
    g = open(destname, "w")
    g.write(content % dict)
    g.close