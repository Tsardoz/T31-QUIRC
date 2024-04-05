#!/bin/sh

# Setting environment variables using fw_setenv
fw_setenv gpio_default 18O 38O 39O 47o 48o 49o 50i 51i 52o 53o 59i 62i 63o 25ID 26IDU
fw_setenv gpio_default_net 48o
fw_setenv gpio_button 51
fw_setenv gpio_mmc_power 48o
fw_setenv gpio_mmc_cd 59
fw_setenv gpio_wlan 57O
fw_setenv gpio_ircut 53 52
fw_setenv gpio_ir850 47
fw_setenv gpio_ir940 49
fw_setenv wlandev 8189fs
fw_setenv disable_eth true
fw_setenv enable_updates true
fw_setenv wlanssid TelstraA60CF8
fw_setenv wlanpass tnd2f99fh7

echo "Environment variables have been set."