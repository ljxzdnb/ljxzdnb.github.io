#!/bin/bash
grep -nr static | awk -F ":" '{print$1}' | xargs sed -i "s/static/static/g"
grep -nr sources | awk -F ":" '{print$1}' | xargs sed -i "s/sources/sources/g"
mv docs/build/html/static docs/build/html/static
mv docs/build/html/sources docs/build/html/sources
