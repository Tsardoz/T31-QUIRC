cmd_/home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/raid/.install := /bin/bash scripts/headers_install.sh /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/raid   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/raid/md_p.h   /home/runner/output/toolchain_xburst1_musl_gcc13/build/linux-headers-custom/include/uapi/linux/raid/md_u.h ; for F in ; do echo "#include <asm-generic/$$F>" > /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/raid/$$F; done; touch /home/runner/output/toolchain_xburst1_musl_gcc13/host/mipsel-thingino-linux-musl/sysroot/usr/include/linux/raid/.install