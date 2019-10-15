#!/bin/sh
set -e
make test
./tools/test_runner.lua $@
