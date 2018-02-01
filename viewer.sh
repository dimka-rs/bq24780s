#!/bin/sh

gcc ./viewer/viewer.c ./viewer/serial.c -lncurses -o build/viewer
