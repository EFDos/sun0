#!/bin/bash

find . -type d -name '*' -exec mkdir -p ../../include/sun0/{} \;
find . -type f -name '*.hpp' -exec cp {} ../../include/sun0/{} \;
find . -type f -name '*.hpp.in' -exec cp {} ../../include/sun0/{} \;
