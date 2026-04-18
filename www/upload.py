#!/usr/bin/env python3
"""
cgi-bin/upload.py
Handles multipart/form-data POST from the upload form.
Saves uploaded files to ../uploads/ relative to the web root.

Your webserv config must:
  - route POST /cgi-bin/upload.py to this script
  - set UPLOAD_DIR or use the default path below
"""

import cgi
import cgitb
import os
import sys
import html
import time

cgitb.enable()   # detailed error output — disable in prod

UPLOAD_DIR = os.environ.get('UPLOAD_DIR', os.path.join(os.path.dirname(__file__), '..', 'uploads'))
MAX_SIZE   = 10 * 1024 * 1024   # 10 MB hard limit

def respond(status, title, message, extra=''):
	body = f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>{html.escape(title)} — webserv</title>
  <meta http-equiv="refresh" content="2;url=/">
  <style>
    *{{box-sizing:border-box;margin:0;padding:0}}
    body{{background:#0e0e0e;color:#d4d4d4;font-family:'IBM Plex Mono',monospace;
          display:flex;align-items:center;justify-content:center;min-height:100vh;padding:24px}}
    .box{{background:#141414;border:1px solid #2a2a2a;border-radius:4px;padding:32px 36px;max-width:480px;width:100%}}
    h1{{font-size:15px;margin-bottom:12px;color:{'#c8f135' if status==200 else '#ff4545'}}}
    p{{font-size:13px;color:#888;line-height:1.7}}
    a{{color:#c8f135;text-decoration:none}}
  </style>
</head>
<body>
<div class="box">
  <h1>{'✓' if status==200 else '✗'} {html.escape(title)}</h1>
  <p>{message}</p>
  {extra}
  <p style="margin-top:20px"><a href="/">← back to test suite</a> (redirecting in 2s)</p>
</div>
</body>
</html>"""
	print(f"Status: {status}")
	print("Content-Type: text/html\r")
	print(f"Content-Length: {len(body.encode())}\r")
	print("\r")
	print(body)

def main():
	# Only accept POST
	if os.environ.get('REQUEST_METHOD', 'GET') != 'POST':
		respond(405, 'Method Not Allowed', 'This endpoint only accepts POST requests.')
		return

	# Ensure upload dir exists
	os.makedirs(UPLOAD_DIR, exist_ok=True)

	# Parse multipart form
	form = cgi.FieldStorage(
		fp=sys.stdin.buffer,
		environ=os.environ,
		keep_blank_values=True
	)

	if 'file' not in form:
		respond(400, 'Bad Request', 'No file field found in the form data.')
		return

	fileitem = form['file']

	if not fileitem.filename:
		respond(400, 'Bad Request', 'No file was selected.')
		return

	# Sanitise filename — strip path components
	filename  = os.path.basename(fileitem.filename.replace('\\', '/'))
	if not filename:
		respond(400, 'Bad Request', 'Invalid filename.')
		return

	# Add timestamp to avoid collisions
	ts       = int(time.time())
	savename = f"{ts}_{filename}"
	savepath = os.path.join(UPLOAD_DIR, savename)

	data = fileitem.file.read(MAX_SIZE + 1)
	if len(data) > MAX_SIZE:
		respond(413, 'Payload Too Large', f'File exceeds the {MAX_SIZE // 1024 // 1024} MB limit.')
		return

	with open(savepath, 'wb') as f:
		f.write(data)

	note = html.escape(form.getvalue('note', ''))
	size = len(data)

	extra = f'<p style="margin-top:10px">Saved as <code>{html.escape(savename)}</code> ({size:,} bytes)</p>'
	if note:
		extra += f'<p>Note: {note}</p>'

	respond(200, 'Upload successful', f'File <code>{html.escape(filename)}</code> was uploaded successfully.', extra)

main()
