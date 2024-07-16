#!/usr/bin/python3

import os
import cgitb
import sqlite3

# Enable error handling
cgitb.enable()

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

print("Content-Type: text/html")
print()
print("<html>")
print("<head>")
print("<title>View Cookies</title>")
print("</head>")
print("<body>")
print("<h2>Stored Cookie Data</h2>")

# Retrieve all key-value pairs for the user's session from the database
conn = sqlite3.connect(db_path)
c = conn.cursor()
c.execute('SELECT key, value FROM kv_store WHERE cookie = ?', (session,))
rows = c.fetchall()
conn.close()

if rows:
    print("<table border='1'>")
    print("<tr><th>Key</th><th>Value</th></tr>")
    for key, value in rows:
        print(f"<tr><td>{key}</td><td>{value}</td></tr>")
    print("</table>")
else:
    print("<p>No data found for your session.</p>")

print("<a href='/pages/cookie/testCookies.html'>Go back to form</a>")
print("</body>")
print("</html>")