#!/bin/bash

# Add to path only if it's not already there to avoid duplication
if [[ ":$PATH:" != *":/home/tsardoz/T31-QUIRC/sdk/host/bin/:"* ]]; then
    export PATH="/home/tsardoz/T31-QUIRC/sdk/host/bin/:$PATH"
fi

# Toolchain specifics
export CC=mipsel-linux-gcc
export CXX=mipsel-linux-g++
export AR=mipsel-linux-ar
export AS=mipsel-linux-as
export LD=mipsel-linux-ld
export STRIP=mipsel-linux-strip
export RANLIB=mipsel-linux-ranlib

# Compiler and linker flags
export CFLAGS="-march=mips32 -mtune=mips32 -O2 \
-I/home/tsardoz/T31-QUIRC/sdk/host/mipsel-buildroot-linux-musl/sysroot/usr/include \
-I/home/tsardoz/T31-QUIRC/sdk/host/include \
-I/home/tsardoz/T31-QUIRC/sdk/host/opt/ext-toolchain/include"

export LDFLAGS="-L/home/tsardoz/T31-QUIRC/sdk/host/mipsel-buildroot-linux-musl/sysroot/usr/lib \
-L/home/tsardoz/T31-QUIRC/sdk/host/opt/ext-toolchain/mipsel-thingino-linux-musl/sysroot/lib \
-L/home/tsardoz/T31-QUIRC/sdk/target/usr/lib"

# Package configuration paths
export PKG_CONFIG_PATH="/home/tsardoz/T31-QUIRC/sdk/host/lib/pkgconfig:/home/tsardoz/T31-QUIRC/sdk/host/opt/ext-toolchain/lib/pkgconfig:$PKG_CONFIG_PATH"

# Library paths
export LD_LIBRARY_PATH="/home/tsardoz/T31-QUIRC/sdk/host/mipsel-buildroot-linux-musl/sysroot/lib:/home/tsardoz/T31-QUIRC/sdk/host/lib:$LD_LIBRARY_PATH"

echo "Cross-compilation environment setup complete."
