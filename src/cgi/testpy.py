import Apache

print "Content-type: text/html"
print

r = Apache.request
print "Request's doc: ", r.__doc__
print "Request's text: ", r.the_request
print "Request's members: ", r.__members__
print "Remote host: ", r.get_remote_host()
try:
    print r.get_remote_logname()
except Apache.error, m:
    print "Cannot get remote logname: ", m

c = r.connection
print "Connection's doc: ", c.__doc__
print "Connection's remote IP: ", c.remote_ip
print "Connection's members: ", c.__members__

print "Headers in: ", r.headers_in.items()
print "Headers out: ", r.headers_out.items()
print "Err Headers out: ", r.err_headers_out.items()
print "Subprocess env: ", r.subprocess_env.items()
print "Notes: ", r.notes.items()

s = c.server
print "Server's doc: ", s.__doc__
print "Server's admin: ", s.server_admin
print "Server's members: ", s.__members__
