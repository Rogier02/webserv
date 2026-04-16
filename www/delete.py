#!/usr/bin/env python3
"""
cgi-bin/delete.py
Receives a POST with field 'path', deletes the file, returns a result page.
Used by the no-JS test suite because browsers cannot send DELETE natively.
"""
 
import cgi
import cgitb
import os
import html
 
cgitb.enable()
 
UPLOAD_DIR = os.path.realpath(
	os.environ.get('UPLOAD_DIR', os.path.join(os.path.dirname(__file__), '..', 'uploads'))
)
 
def respond(status, title, message, ok=True):
	color = '#c8f135' if ok else '#ff4545'
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
    h1{{font-size:15px;margin-bottom:12px;color:{color}}}
    p{{font-size:13px;color:#888;line-height:1.7}}
    a{{color:#c8f135;text-decoration:none}}
    code{{background:#1e1e1e;padding:1px 5px;border-radius:2px;font-size:12px}}
  </style>
</head>
<body>
<div class="box">
  <h1>{'✓' if ok else '✗'} {html.escape(title)}</h1>
  <p>{message}</p>
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
	if os.environ.get('REQUEST_METHOD', '') != 'POST':
		respond(405, 'Method Not Allowed', 'This endpoint only accepts POST.', ok=False)
		return
 
	form = cgi.FieldStorage()
	path = form.getvalue('path', '').strip()
 
	if not path:
		respond(400, 'Bad Request', 'No path provided.', ok=False)
		return
 
	# Resolve and confirm it stays inside UPLOAD_DIR
	target = os.path.realpath(os.path.join(UPLOAD_DIR, os.path.basename(path)))
 
	if not target.startswith(UPLOAD_DIR + os.sep):
		respond(403, 'Forbidden', 'Path is outside the uploads directory.', ok=False)
		return
 
	if not os.path.isfile(target):
		respond(404, 'Not Found', f'No file found at <code>{html.escape(path)}</code>.', ok=False)
		return
 
	os.remove(target)
	respond(200, 'Deleted', f'File <code>{html.escape(os.path.basename(target))}</code> was deleted.', ok=True)
 
main()