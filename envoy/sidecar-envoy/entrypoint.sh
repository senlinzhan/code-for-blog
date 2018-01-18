#!/bin/sh

python app.py &
/usr/local/bin/envoy -c /etc/envoy.json

