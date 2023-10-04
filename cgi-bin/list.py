#!/usr/bin/env python3

import cgi
import json
import os

print("Content-Type: application/json")
print()

directory = os.path.join("uploads")
os.listdir(directory)

data = {}
data["files"] = os.listdir(directory);

print(json.dumps(data))
