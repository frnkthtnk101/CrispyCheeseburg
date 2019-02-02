# redirect sys.stdout to a buffer
import sys, io
stdout = sys.stdout
sys.stdout = io.StringIO()

# call module that calls print()
print("hey")

# get output and restore sys.stdout
output = sys.stdout.getvalue()
sys.stdout = stdout

print(output)