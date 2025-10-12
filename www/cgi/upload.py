#!/usr/bin/env python3
import sys
import os
import cgi

print("Content-Type: text/html")
print()
print("<html><head><title>File Upload</title></head><body>")
print("<h1>File Upload Handler</h1>")

if os.environ.get('REQUEST_METHOD') == 'POST':
    print("<p class='success'>POST request received!</p>")
    content_length = int(os.environ.get('CONTENT_LENGTH', 0))
    if content_length > 0:
        post_data = sys.stdin.read(content_length)
        print(f"<p>Received {content_length} bytes of data.</p>")
        print("<p>Upload processing would happen here.</p>")
else:
    print("<p>No file uploaded. Use POST method.</p>")

print("<p><a href='/upload.html'>Upload another file</a></p>")
print("<p><a href='/'>Back to home</a></p>")
print("</body></html>")
