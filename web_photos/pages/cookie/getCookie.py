# #! /usr/bin/python3

# import os
# from http import cookies
# # Import modules for CGI handling
# import cgi, cgitb

# # Create instance of FieldStorage
# form = cgi.FieldStorage()

# # Get data from fields
# key = form.getvalue('name')
# cookie = cookies.SimpleCookie()
# if 'HTTP_COOKIE' in os.environ:
#     cookie.load(os.environ["HTTP_COOKIE"])
# if key in cookie:
#     print("HTTP/1.1 200 OK")
#     print("Content-Type: text/plain\r\n")
#     print("The Value of Cookie", key, "is", cookie[key].value)
# else:
#     print("HTTP/1.1 200 OK")
#     print("Content-Type: text/plain\r\n")
#     print("Cookie was not found !")

#! /usr/bin/python3

import os
from http import cookies
import cgi, cgitb

# Enable error handling
cgitb.enable()

# Get the cookies from the HTTP header
cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE'))

print("Content-Type: text/html")
print("\r\n")
print("<html>")
print("<head>")
print("<title>Cookie Value</title>")
print("</head>")
print("<body>")
print("<h2>Cookie Value</h2>")

# Display all cookies
if cookie:
    for key in cookie:
        print(f"<p>{key}: {cookie[key].value}</p>")
else:
    print("<p>No cookies found</p>")

print("<a href='/pages/cookie/testCookies.html'>Go back to form</a>")
print("</body>")
print("</html>")
