import sys
import os
sym=sys.argv[1]
dname="/opt/pleora/ebus_sdk/Ubuntu-12.04-x86_64/lib"
libs=os.listdir(dname)
for lib in libs:
    if lib.endswith(".so"):
        lines=os.popen("nm -D --defined-only %s"%os.path.join(dname,lib)).readlines()
        for line in lines:
            if sym in line:
                print lib
