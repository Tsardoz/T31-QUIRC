


# OPENCV_INCLUDE := $(which mipsel-linux-g++)
GPP_LOCATION := $(shell which mipsel-linux-g++)
OPENCV_INCLUDE := $(shell dirname ${GPP_LOCATION})/../mipsel-openipc-linux-musl/sysroot/usr/include/opencv4

t31:
	mipsel-linux-g++ -I./third-party -I$(OPENCV_INCLUDE) -L./objects/t31 main.cpp ./backends/t31-wyze.cpp qr.cpp encryption.cpp network.cpp ./third-party/base64/base64.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lcrypto -lcurl -lquirc -lpthread -o Safest

dev:
	mipsel-linux-g++ -I./third-party -I/usr/local/include -I/usr/local/include/opencv4 main.cpp ./backends/test.cpp qr.cpp encryption.cpp network.cpp ./third-party/base64/base64.cpp -lopencv_core -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lcrypto -lcurl -lquirc -lpthread -o Safest