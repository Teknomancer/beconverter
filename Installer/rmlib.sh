#!/bin/sh

# Script that removes libBeConverter.so from ~/config/lib.
# As BeConverter from version 1.2 upwards puts the lib
# in the "lib" folder of the app itself.
# BeConverter (c) 2002 Ramshankar.

rm /boot/home/config/lib/libBeConverter.so
