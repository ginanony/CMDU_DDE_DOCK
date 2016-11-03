#!/bin/bash
sudo cp `dirname $0`/libMyPlugin.so /usr/lib/dde-dock/plugins/libMyPlugin.so
killall dde-dock
