#!/usr/bin/python3

import os
import cgi
import cgitb
import sqlite3

# Enable error handling
cgitb.enable()

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
key = form.getvalue('name')
value = form.getvalue('value')

# Path to the SQLite database file
db_path = 'database.db'

# Function to initialize the database
def init_db():
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    c.execute('''
        CREATE TABLE IF NOT EXISTS kv_store (
            cookie TEXT,
            key TEXT,
            value TEXT,
            PRIMARY KEY (cookie, key)
        )
    ''')
    conn.commit()
    conn.close()

# Initialize the database
init_db()

# Load the cookie value directly from HTTP_COOKIE
if 'HTTP_COOKIE' in os.environ:
    session = os.environ['HTTP_COOKIE']
else:
    print("Content-Type: text/html")
    print()
    print("<html>")
    print("<head>")
    print("<title>Error</title>")
    print("</head>")
    print("<body>")
    print("<h2>Error: No session cookie found</h2>")
    print("<a href='/pages/cookie/testCookies.html'>Go back to form</a>")
    print("</body>")
    print("</html>")
    exit()

# Update the database with the new key-value pair
if key and value:
    try:
        conn = sqlite3.connect(db_path)
        c = conn.cursor()
        c.execute('REPLACE INTO kv_store (cookie, key, value) VALUES (?, ?, ?)', (session, key, value))
        conn.commit()
        conn.close()
        db_update_success = True
    except Exception as e:
        db_update_success = False
        error_message = str(e)
else:
    db_update_success = False
    error_message = "Key or Value missing"

print("Content-Type: text/html")
print()
print("<html>")
print("<head>")
print("<title>Cookie Set</title>")
print("</head>")
print("<body>")
if db_update_success:
    print("<h2>Key-Value pair has been set</h2>")
else:
    print(f"<h2>Error: {error_message}</h2>")
print("<a href='/pages/cookie/testCookies.html'>Go back to form</a>")
print("</body>")
print("</html>")
