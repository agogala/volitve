# $ProjectHeader: volitve 0.16 Tue, 30 Sep 1997 18:27:41 +0200 andrej $
#
# $Id: Util.py 1.1 Thu, 11 Sep 1997 21:18:12 +0000 andrej $
#
# Razlièni podprogrami


def MakeTemplate(templname, destname, dict):
    # Preberi obrazec:
    f = open(templname, "r")
    content = f.read()
    f.close();
    g = open(destname, "w")
    g.write(content % dict)
    g.close
