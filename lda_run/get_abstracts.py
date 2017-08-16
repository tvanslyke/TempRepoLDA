import json
from sys import stdin, stdout

for line in stdin:
	entry = json.loads(line.strip())
	stdout.write(json.dumps(entry["paperAbstract"]))
	stdout.write("\n")
