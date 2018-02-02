#!/bin/sh

gcc -g ./viewer/viewer.c ./viewer/serial.c -lncurses -o build/viewer
