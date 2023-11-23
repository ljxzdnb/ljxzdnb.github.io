#!/bin/bash
find  build/html/  -type f -regex  ".*\.\(html\|css\|js\)" | xargs grep -nr static | awk -F ":" '{print$1}' | xargs sed -i "s/_static/static/g"
find  build/html/  -type f -regex  ".*\.\(html\|css\|js\)" | xargs grep -nr sources | awk -F ":" '{print$1}' | xargs sed -i "s/_sources/sources/g"
mv build/html/_static build/html/static
mv build/html/_sources build/html/sources
