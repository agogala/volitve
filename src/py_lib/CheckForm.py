# $ProjectHeader: volitve 0.28 Sat, 27 Dec 1997 16:06:49 +0100 andrej $
#
# $Id: CheckForm.py 1.2 Fri, 05 Sep 1997 14:43:33 +0000 andrej $
#
# Preveri veljavnost obrazca:

def CheckForm(form, fields, reqfields):
    form_ok = 1
    for f in fields:
	form_ok = form_ok and form.has_key(f)
	if not form_ok:
	    break
    if form_ok:
	for rf in reqfields:
	    form_ok = form_ok and form[rf] != ""
	    if not form_ok:
		break
    return form_ok
