#!/bin/sh
router="$(ip route show | grep -i 'default via'| awk '{print $3 }')"
if ping -c 4 "$router" >> /dev/null; then
    echo "able to ping router"
    exit 0
fi

if nc -w1 pilotservices.pages.dev 443; then
    echo "able to get backend"
    exit 0
fi

echo "unable to find connection"
exit 1