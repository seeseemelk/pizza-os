#!/bin/sh
set -e
./build.sh
bochs -q
