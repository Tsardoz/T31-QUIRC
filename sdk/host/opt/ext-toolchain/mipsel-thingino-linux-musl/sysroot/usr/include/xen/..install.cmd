cmd_/home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/xen/.install := /bin/bash scripts/headers_install.sh /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/xen   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/xen/evtchn.h   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/xen/privcmd.h ; for F in ; do echo "#include <asm-generic/$$F>" > /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/xen/$$F; done; touch /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/xen/.install