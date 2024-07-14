#! /usr/bin/python3

import os
from http import cookies
import cgi, cgitb

# Enable error handling
cgitb.enable()

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
key = form.getvalue('name')
value  = form.getvalue('value')
# Create and set the cookie

cookie = cookies.SimpleCookie()
cookie[key] = value

print("Content-Type: text/html")
print(cookie.output())
print("\r\n")
print("<html>")
print("<head>")
print("<title>Cookie Set</title>")
print("</head>")
print("<body>")
print("<h2>Cookie has been set</h2>")
print("<a href='/pages/cookie/testCookies.html'>Go back to form</a>")
print("</body>")
print("</html>")
