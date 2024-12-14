import random
from random import randint
import subprocess

filename = "./cmake-build-debug/main"

names = ["DOUBLE", "FLOAT", "FIXED(32,16)", "FAST_FIXED(48,16)"]

for a in range(4):
    for b in range(4):
        for c in range(4):
            inp = f"--p-type={names[a]} --v-type={names[b]} --vf-type={names[c]} --in-file=./TestField.txt"

            try:
                output = subprocess.run([filename, inp], capture_output=True, timeout=10)
                print(names[a], names[b], names[c])
            except subprocess.TimeoutExpired:
                print("ok")

print("finish")
