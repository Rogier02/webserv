#!/usr/bin/env python3

from datetime import datetime

current_time = datetime.now()
print(f"<H1>Current time using Python: {current_time.strftime('%d-%m-%Y %H:%M:%S')}</H1>")