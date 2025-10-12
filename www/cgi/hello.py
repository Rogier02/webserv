#!/usr/bin/env python3
import sys
import os

print("Content-Type: text/html")
print()
print("<html><head><title>Python CGI</title></head><body>")
print("<h1>Hello from Python CGI!</h1>")
print("<p>CGI is working correctly.</p>")

if os.environ.get('REQUEST_METHOD') == 'POST':
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    if content_length > 0:
        post_data = sys.stdin.read(content_length)
        print(f"<p><strong>POST data received:</strong> {post_data}</p>")

print("<p><strong>Environment:</strong></p>")
print("<ul>")
print(f"<li>REQUEST_METHOD: {os.environ.get('REQUEST_METHOD', 'N/A')}</li>")
print(f"<li>PATH_INFO: {os.environ.get('PATH_INFO', 'N/A')}</li>")
print(f"<li>QUERY_STRING: {os.environ.get('QUERY_STRING', 'N/A')}</li>")
print("</ul>")
print("<p><a href='/'>Back to home</a></p>")
print("</body></html>")
