#!/usr/bin/env python3

import time
from os import listdir, system, path
import random
import sys

def main():
    #sorta improving the randomness for the value by relying on /dev/urandom
    seed_val = 0
    with open("/dev/urandom", 'rb') as rng:
        seed_val = int.from_bytes(rng.read(8),"little")
        #print(f"seed value: {seed_val}")
    random.seed(seed_val)
    waifu_dir = sys.argv[1]

    if not sys.argv[1]:
        print("Usage: ./run.py <waifu directory>")
        return

    files = listdir(sys.argv[1])
    files = [file for file in files if ".png" in file]
    if not files:
        print(f"No waifus in directory: {waifu_dir}")
        return

    while(True):
        f = files
        random.shuffle(f)
        file = f.pop()
        system(f"/opt/alert_widget {path.join(waifu_dir, file)} &")
        time.sleep(60)
        system("pkill -9 alert_widget")
        time.sleep(60*29)

if __name__ == "__main__":
    main()
