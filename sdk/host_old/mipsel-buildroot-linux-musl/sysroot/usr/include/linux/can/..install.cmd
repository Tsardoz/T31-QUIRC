cmd_/home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/can/.install := /bin/bash scripts/headers_install.sh /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/can   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/can/bcm.h   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/can/error.h   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/can/gw.h   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/can/netlink.h   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/can/raw.h ; for F in ; do echo "#include <asm-generic/$$F>" > /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/can/$$F; done; touch /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/can/.install