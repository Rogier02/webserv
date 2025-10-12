#!/bin/bash
echo "Content-Type: text/html"
echo ""
echo "<html><head><title>Bash CGI</title></head><body>"
echo "<h1>Hello from Bash CGI!</h1>"
echo "<p>This is a simple bash CGI script.</p>"
echo "<p><strong>Server Time:</strong> $(date)</p>"
echo "<p><a href='/'>Back to home</a></p>"
echo "</body></html>"
