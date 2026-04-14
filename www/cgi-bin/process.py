#!/usr/bin/env python3

import sys
import os
import json
import urllib.parse
import html
from pathlib import Path
from datetime import datetime

def emit_headers(content_type="text/html", extra_headers=None):
  sys.stdout.write(f"Content-Type: {content_type}\r\n")
  if extra_headers:
    for key, value in extra_headers.items():
      sys.stdout.write(f"{key}: {value}\r\n")
  sys.stdout.write("\r\n")


def submissions_path():
  return Path(__file__).resolve().parent / "server_1" / "forms" / "submissions.txt"


def load_posts(submissions):
  if not submissions.exists():
    return []

  posts = []
  current = {}
  collecting_message = False

  with submissions.open("r") as f:
    for raw_line in f:
      line = raw_line.rstrip("\n")

      if line.startswith("Time: "):
        current["time"] = line[6:]
        collecting_message = False
      elif line.startswith("Name: "):
        current["name"] = line[6:]
        collecting_message = False
      elif line.startswith("Email: "):
        current["email"] = line[7:]
        collecting_message = False
      elif line.startswith("Message: "):
        current["message"] = line[9:]
        collecting_message = True
      elif line.strip() == "---":
        posts.append({
          "time": current.get("time", ""),
          "name": current.get("name", ""),
          "email": current.get("email", ""),
          "message": current.get("message", "")
        })
        current = {}
        collecting_message = False
      elif collecting_message:
        current["message"] = current.get("message", "") + "\n" + line

  if current:
    posts.append({
      "time": current.get("time", ""),
      "name": current.get("name", ""),
      "email": current.get("email", ""),
      "message": current.get("message", "")
    })

  return posts

def main():
    method = os.environ.get("REQUEST_METHOD", "GET").upper()
    submissions = submissions_path()

    if method == "GET":
        query = urllib.parse.parse_qs(os.environ.get("QUERY_STRING", ""))
        if "posts" in query:
            emit_headers("application/json")
            print(json.dumps({"posts": load_posts(submissions)}))
            return

        emit_headers()
        print("<h1>Method Not Allowed</h1><p>Use POST to submit or GET with ?posts=1 to list posts.</p>")
        return

    raw_input = sys.stdin.read()
    form_data = urllib.parse.parse_qs(raw_input)
    name = form_data.get('name', [''])[0]
    email = form_data.get('email', [''])[0]
    message = form_data.get('message', [''])[0]
    timestamp = datetime.utcnow().isoformat(timespec="seconds") + "Z"

    submissions.parent.mkdir(parents=True, exist_ok=True)
    with submissions.open("a") as f:
        f.write(
            f"Time: {timestamp}\n"
            f"Name: {name}\n"
            f"Email: {email}\n"
            f"Message: {message}\n"
            "---\n\n"
        )

    emit_headers("application/json")
    print(json.dumps({"ok": True, "message": "success! :)"}))

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        emit_headers()
        print("<h1>Form Submission Failed</h1>")
        print(f"<pre>{html.escape(str(e))}</pre>")
