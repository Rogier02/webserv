#!/usr/bin/env python3

from datetime import datetime

# Get the current time
current_time = datetime.now()
timestamp = current_time.strftime('%d-%m-%Y %H:%M:%S')

# Generate HTML with embedded styling
html = f"""<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<title>timestamp.py</title>
	<style>
		* {{ box-sizing: border-box; margin: 0; padding: 0; }}
		body {{
			background: #0e0e0e;
			color: #fff1f1;
			font-family: 'IBM Plex Mono', monospace;
			display: flex;
			flex-direction: column;
			align-items: center;
			justify-content: center;
			min-height: 100vh;
			padding: 24px;
		}}
		.box {{ text-align: center; }}
		.code {{ font-size: 80px; font-weight: 600; color: #1e1e1e; line-height: 1; }}
		.msg  {{ font-size: 14px; color: #555; margin-top: 12px; }}
		a {{
			display: inline-block;
			margin-top: 24px;
			font-size: 13px;
			color: #ff4545;
			text-decoration: none;
			border: 1px solid #6b2020;
			padding: 8px 18px;
			border-radius: 4px;
		}}
		a:hover {{ background: #ff4545; color: #fff; }}
		.tiemestamp {{
			font-size: 24px;
			color: #ff4545;
			font-family: 'IBM Plex Mono', monospace;
			text-align: center;
			margin-bottom: 24px;
			margin-top: 42px;
		}}
	</style>
</head>
<body>
	<div class="box">
		<div class="code">Timestamp.py</div>
	</div>
	<p style="margin-bottom:16px; color: whitesmoke ;"><a href="/">← back</a></p>
	<div class="box">
		<div class=timestamp>{timestamp}</div>
	</div>
</body>
</html>"""

print("Content-Type: text/html\r")
print(f"Content-Length: {len(html.encode())}\r")
print("\r")
print(html)