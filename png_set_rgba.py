#!/usr/bin/env python3

import sys
from PIL import Image

def main():
    if not sys.argv[1]:
        print("Usage: ./png_set_rgba.py <PNG FILE>")
        return
    path = sys.argv[1];
    print(path)
    Image.open(path).convert('RGBA').save(path)

if __name__ == "__main__":
    main()
