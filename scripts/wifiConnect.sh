#!/bin/sh
if [ $# -lt 2 ] 
then
echo "Usage: ./wifi.sh <ssid> <passkey>"
exit 1
fi

SSID=$1
PASSKEY=$2

#Set just in case it doesnt exist
fw_setenv wlandev "8189fs"

#Kill the current wifi connection if any exists
killall -q wpa_supplicant
# killall -q udhcpc

wpa_passphrase "$SSID" "$PASSKEY" > /tmp/wpa_supplicant_tmp.conf

#Try to connect and launch in background - wait 10 seconds for connection
wpa_supplicant -B -D nl80211 -i wlan0 -c /tmp/wpa_supplicant_tmp.conf -P /var/run/wpa_supplicant.wlan0.pid

#TODO Test
udhcpc -x hostname:thingino -S -T1 -t15 -R -b -O search -p /var/run/udhcpc.wlan0.pid -i wlan0

sleep 10

sh ./wifiCheck.sh

if [ $? -gt 0 ]
then
    exit 1
fi

echo "Success: $?"

fw_setenv wlanssid "$SSID"
fw_setenv wlanpass "$PASSKEY"
exit 0