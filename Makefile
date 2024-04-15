DEBUG=1
CROSS_COMPILE=./sdk/host/bin/mipsel-linux-
THIRD_PARTY_INCLUDE_DIR=./third-party
# uclibc libraries and includes for SDK
SDK_LIB_DIR=./sdk/lib/uclibc
SDK_INC_DIR=./sdk/include
INCLUDES = -I$(SDK_INC_DIR) -I$(THIRD_PARTY_INCLUDE_DIR) -Iquirc
SDK_LIBS = $(SDK_LIB_DIR)/libimp.so $(SDK_LIB_DIR)/libalog.so
LDFLAGS += -Wl,--unresolved-symbols=ignore-in-shared-libs
LDFLAGS += -Wl,--dynamic-linker=/usr/lib/ld-musl-mipsel.so.1
OPENCV_INC_DIR = ./sdk/host/mipsel-buildroot-linux-musl/sysroot/usr/include/opencv4
OPENCV_LIB_DIR = ./sdk/host/mipsel-buildroot-linux-musl/sysroot/usr/lib/
OPENCV_LIBS = -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_highgui -lopencv_video



CC = $(CROSS_COMPILE)gcc
CPLUSPLUS = $(CROSS_COMPILE)g++
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

image-processing.o: image-processing.cpp image-processing.h
	$(CPLUSPLUS) $(CFLAGS) -I$(SDK_INC_DIR) -I$(OPENCV_INC_DIR) \
	-c image-processing.cpp -o image-processing.o

snap-YUV: snap-YUV.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o parse_ini.o image-processing.o
	$(CPLUSPLUS) $(CFLAGS) $(LDFLAGS) -o $@ snap-YUV.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o parse_ini.o image-processing.o \
	-L$(SDK_LIB_DIR) -limp -lalog -L. -lpthread -lm -lrt -lsysutils \
	-L$(OPENCV_LIB_DIR) -lopencv_core -lopencv_imgproc
	if [ "$(DEBUG)" != "1" ]; then $(CROSS_COMPILE)strip $@; fi

#CFLAGS += -I$(OPENCV_INC_DIR) # Include the OpenCV headers
CFLAGS += -I$(OPENCV_INC_DIR)/..

LDFLAGS += -Wl,-Bstatic -L$(OPENCV_LIB_DIR) -lopencv_core -lopencv_imgproc -lopencv_videoio -lopencv_highgui -lopencv_video

# For other libraries that should remain dynamically linked, use -Bdynamic
LDFLAGS += -Wl,-Bdynamic -lpthread -lm -lrt

track: track.o image-processing.o
	$(CPLUSPLUS) $(CFLAGS) $(LDFLAGS) -o $@ track.o image-processing.o \
	$(SDK_LIBS) # Assuming SDK_LIBS are the other libraries linked dynamically
	if [ "$(DEBUG)" != "1" ]; then $(CROSS_COMPILE)strip $@; fi



