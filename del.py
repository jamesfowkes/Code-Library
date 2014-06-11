import os
import sys

for f in sys.argv[1:]:
    try:
        os.remove(f)
    except:
        pass