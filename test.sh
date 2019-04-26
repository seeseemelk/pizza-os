#!/bin/sh
set -e
./build_test.sh
./tools/test_runner.lua
