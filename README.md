# T31-QUIRC
QR code recognition for T31 (Wyze V3)

This uses QUIRC for QR code recognition on T31x using Thingino https://github.com/themactep/thingino-firmware.
By default it uses GC2053 sensor and Realtek wifi.
QUIRC was chosen due to small size and footprint but there are other better ones which are probably slower but more accurate.
You will need to set environment variables to suit your setup or use a UART to connect.

```
fw_setenv wlanpass xxxxx
fw_setenv wlanssid yyyyy
```

There are two makefiles, one for PC and one for T31. This is so you can check results on PC but it will need to be configured to analyse a video.
Sorry this code is a bit messy ATM and you need to know what you are doing to get it working.
I will update this readme later to be more informative.

Start ssh terminal.
Login password 123, user root
To run:
```
cd /overlay/safest
./run_snap.sh
```

With a Wyze V3 set up some QR code on your phone. It is desgned to work up to version 4 (binary/data) but you may get better than that.
Run the program and hold your phone with QR code about 6" from camera.
Move it back and forth until you see a decoded message on your terminal.
It will also save the decoded image to /tmp/snap.yuv
You can scp this across and view it with:

```
scp root@192.168.0.141:/tmp/snap.yuv ./
ffplay -f rawvideo -pixel_format nv12 -video_size 640x360 snap.yuv
```

I designed this to work as fast as possible (almost) so have chosen 640x360 as image size. It runs at about 20FPS this way. To change this check common.c and alter:
```
struct chn_conf chn[FS_CHN_NUM] = {
	{
		.index = CH0_INDEX,
		.enable = CHN0_EN,
    .payloadType = IMP_ENC_PROFILE_HEVC_MAIN,
		.fs_chn_attr = {
			.pixFmt = PIX_FMT_NV12,
			.outFrmRateNum = SENSOR_FRAME_RATE_NUM,
			.outFrmRateDen = SENSOR_FRAME_RATE_DEN,
			.nrVBs = 2,
			.type = FS_PHY_CHANNEL,

			//.scaler.enable = 0,
			.scaler.enable = 1,
			.scaler.outwidth = 640,
			.scaler.outheight = 360,
			//.crop.enable = CROP_EN,
			.crop.enable = 0,
			.crop.top = 0,
			.crop.left = 0,
			.crop.width = SENSOR_WIDTH,
			.crop.height = SENSOR_HEIGHT,

			//.picWidth = SENSOR_WIDTH,
			//.picHeight = SENSOR_HEIGHT,
			.picWidth = 640,
			.picHeight = 360,
		   },
		.framesource_chn =	{ DEV_ID_FS, CH0_INDEX, 0},
		.imp_encoder = { DEV_ID_ENC, CH0_INDEX, 0},
	},
```
to whatever you like eg. 960x540 but note it will run slower, whilst improving detection accuracy per frame.
Also to tweak the camera IMP settings, in snap-YUV.c/main

```
unsigned char aecomp = 100; // Automatic Exposure compensation
	unsigned char contrast = 200;
	unsigned char brightness = 90;
	unsigned char sharpness = 140;
```

these are just preliminary values that seem to work OK.
I would leave the base frame rate of the camera high (eg. >=20 FPS) to avoid motion artifact, even if you skip a few frames due to higher resolution.
I'd be interested to know if people can improve on these settings.

```
make snap-YUV
```
to compile.
This code was based on sample-snap-YUV from Ingenic SDK.

The CLAHE stuff is in there but not used. pyquirc was initially used for testing on pc and worked much better with CLAHE. Real c QUIRC hates CLAHE though so not used.
I thought they would be the same but they are very different from each other.

Thanks to everyone on Teacup discord for their help with this.
