#!/usr/bin/env python3

import time
from os import listdir, system
import random

def main():
    f = listdir("/opt/waifus/")
    random.seed(time.time())
    random.shuffle(f)
    filename = f.pop()
    system(f"/opt/alert_widget /opt/waifus/{filename} &")
    time.sleep(60)
    system("pkill -9 alert_widget")

if __name__ == "__main__":
    main()
