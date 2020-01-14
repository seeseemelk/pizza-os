#!/bin/sh
set -e
make all
bochs -q
