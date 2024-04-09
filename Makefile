#DEBUG=1
CROSS_COMPILE=./sdk/host/bin/mipsel-linux-
THIRD_PARTY_INCLUDE_DIR=./third-party
# uclibc libraries and includes for SDK
SDK_LIB_DIR=./sdk/lib/uclibc
SDK_INC_DIR=./sdk/include
INCLUDES = -I$(SDK_INC_DIR) -I$(THIRD_PARTY_INCLUDE_DIR) -Iquirc
SDK_LIBS = $(SDK_LIB_DIR)/libimp.so $(SDK_LIB_DIR)/libalog.so
LDFLAGS += -Wl,--unresolved-symbols=ignore-in-shared-libs
LDFLAGS += -Wl,--dynamic-linker=/usr/lib/ld-musl-mipsel.so.1

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar
STRIP = $(CROSS_COMPILE)strip

# Set optimisation levels
ifeq ($(DEBUG), 1)
CFLAGS = $(INCLUDES) -O0 -g -Wall -march=mips32r2 -no-pie
else
CFLAGS = $(INCLUDES) -O2 -Wall -march=mips32r2
endif

%.o:%.c common.h
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ *.a

distclean: clean
	rm -f $(SAMPLES)

quirc.o: third-party/quirc/quirc.c
	$(CC) $(CFLAGS) -c third-party/quirc/quirc.c -o quirc.o

parse_ini.o: parse_ini.c
	$(CC) $(CFLAGS) -c parse_ini.c -o parse_ini.o

t31-c-utils.o: t31-c-utils.c
	$(CC) $(CFLAGS) -c t31-c-utils.c -o t31-c-utils.o

setup_camera.o: setup_camera.c
	$(CC) $(CFLAGS) -c setup_camera.c -o setup_camera.o

musl_shim.o: musl_shim.c
	$(CC) $(CFLAGS) -c musl_shim.c -o musl_shim.o

common.o: common.c
	$(CC) $(CFLAGS) -c common.c -o common.o

identify.o: third-party/quirc/identify.c
	$(CC) $(CFLAGS) -c third-party/quirc/identify.c -o identify.o

decode.o: third-party/quirc/decode.c
	$(CC) $(CFLAGS) -c third-party/quirc/decode.c -o decode.o

version_db.o: third-party/quirc/version_db.c
	$(CC) $(CFLAGS) -c third-party/quirc/version_db.c -o version_db.o

snap-YUV: snap-YUV.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o parse_ini.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ snap-YUV.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o parse_ini.o \
	-L$(SDK_LIB_DIR) -limp -lalog -L. $(LIBS) -lpthread -lm -lrt -lsysutils
	@if [ "$(DEBUG)" != "1" ]; then $(STRIP) $@; fi



