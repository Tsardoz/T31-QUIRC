#DEBUG=1
CROSS_COMPILE=./sdk/host/bin/mipsel-linux-
OPENCV_INCLUDE=./sdk/host/mipsel-buildroot-linux-musl/sysroot/usr/include/opencv4/
THIRD_PARTY_INCLUDE_DIR=./third-party
OPENCV_LIB_DIR=./sdk/target/usr/lib/
# uclibc libraries and includes for SDK
SDK_LIB_DIR=./sdk/lib/uclibc
SDK_INC_DIR=./sdk/include
INCLUDES = -I$(SDK_INC_DIR) -I$(THIRD_PARTY_INCLUDE_DIR) -I$(OPENCV_INCLUDE) -Iquirc
SDK_LIBS = $(SDK_LIB_DIR)/libimp.so $(SDK_LIB_DIR)/libalog.so
LDFLAGS += -Wl,--unresolved-symbols=ignore-in-shared-libs
LDFLAGS += -Wl,--dynamic-linker=/usr/lib/ld-musl-mipsel.so.1
LIBS += -L$(OPENCV_LIB_DIR) -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

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

t31:
	mipsel-linux-g++ -I./third-party -I$(OPENCV_INCLUDE) -L./objects/t31 main.cpp ./backends/t31-wyze.cpp qr.cpp encryption.cpp network.cpp ./third-party/base64/base64.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lcrypto -lcurl -lquirc -lpthread -O0 -o Safest

dev:
	g++ -I./third-party -I/usr/local/include -I/usr/local/include/opencv4 main.cpp ./backends/test.cpp qr.cpp encryption.cpp network.cpp ./third-party/base64/base64.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lcrypto -lcurl -lquirc -lpthread -o Safest

%.o:%.c common.h
	$(CC) -c $(CFLAGS) $< -o $@

%.o:%.cpp
	$(CPLUSPLUS) -c $(CFLAGS) $< -o $@

clean:
	rm -f *.o *~ *.a

distclean: clean
	rm -f $(SAMPLES)

t31-wyze.o: t31-wyze.cpp
	$(CPLUSPLUS) $(CFLAGS) -c t31-wyze.cpp -o t31-wyze.o

qr.o: qr.cpp
	$(CPLUSPLUS) $(CFLAGS) -c qr.cpp -o qr.o

quirc.o: third-party/quirc/quirc.c
	$(CC) $(CFLAGS) -c third-party/quirc/quirc.c -o quirc.o

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

main: main.o t31-wyze.o qr.o quirc.o identify.o decode.o version_db.o setup_camera.o musl_shim.o common.o
	$(CPLUSPLUS) $(CFLAGS) $(LDFLAGS) -o $@ main.o t31-wyze.o qr.o quirc.o identify.o decode.o version_db.o setup_camera.o musl_shim.o common.o \
	-L$(OPENCV_LIB_DIR) -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs \
	-L$(SDK_LIB_DIR) -limp -lalog -L. $(LIBS) -lpthread -lm -lrt
	@if [ "$(DEBUG)" != "1" ]; then $(STRIP) $@; fi

snap-YUV: snap-YUV.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o
	$(CPLUSPLUS) $(CFLAGS) $(LDFLAGS) -o $@ snap-YUV.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o \
	-L$(OPENCV_LIB_DIR) -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs \
	-L$(SDK_LIB_DIR) -limp -lalog -L. $(LIBS) -lpthread -lm -lrt -lsysutils
	@if [ "$(DEBUG)" != "1" ]; then $(STRIP) $@; fi

clahe: clahe.o t31-c-utils.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o
	$(CPLUSPLUS) $(CFLAGS) $(LDFLAGS) -o $@ clahe.o identify.o decode.o version_db.o musl_shim.o common.o t31-c-utils.o quirc.o \
	-L$(OPENCV_LIB_DIR) -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs \
	-L$(SDK_LIB_DIR) -limp -lalog -L. $(LIBS) -lpthread -lm -lrt -lsysutils
	@if [ "$(DEBUG)" != "1" ]; then $(STRIP) $@; fi


