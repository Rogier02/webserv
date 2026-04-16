#!/usr/bin/env python3
"""
cgi-bin/env.py
Dumps all CGI environment variables — useful for verifying your
server correctly sets REQUEST_METHOD, CONTENT_TYPE, PATH_INFO, etc.
"""

import os
import html

vars_sorted = sorted(os.environ.items())

rows = ''.join(
	f'<tr><td class="key">{html.escape(k)}</td><td class="val">{html.escape(v)}</td></tr>'
	for k, v in vars_sorted
)

body = f"""<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>CGI env — webserv</title>
  <style>
    *{{box-sizing:border-box;margin:0;padding:0}}
    body{{background:#0e0e0e;color:#d4d4d4;font-family:'IBM Plex Mono',monospace;padding:40px 24px}}
    h1{{font-size:15px;color:#c8f135;margin-bottom:20px}}
    a{{color:#7a9420;font-size:12px}}
    table{{border-collapse:collapse;width:100%;max-width:860px}}
    tr{{border-bottom:1px solid #1e1e1e}}
    tr:hover{{background:#141414}}
    td{{padding:8px 12px;font-size:12px;vertical-align:top}}
    .key{{color:#888;width:260px;white-space:nowrap}}
    .val{{color:#d4d4d4;word-break:break-all}}
  </style>
</head>
<body>
  <h1>CGI environment</h1>
  <p style="margin-bottom:16px"><a href="/">← back</a></p>
  <table>
    <tbody>{rows}</tbody>
  </table>
</body>
</html>"""

print("Content-Type: text/html\r")
print(f"Content-Length: {len(body.encode())}\r")
print("\r")
print(body)