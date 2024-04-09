/*
 * sample-snap-raw.c
 *
 * Copyright (C) 2018 Ingenic Semiconductor Co.,Ltd
 */

#include <ctype.h>
#include <stdlib.h>
#include <imp/imp_log.h>
#include <imp/imp_common.h>
#include <imp/imp_system.h>
#include <imp/imp_framesource.h>
#include <imp/imp_encoder.h>
#include <string.h>

#include "common.h"
#include "t31-c-utils.h"
#include "parse_ini.h"
#define TAG "Snap-YUV"
extern struct chn_conf chn[];
SensorConfig config;

INIEntry entries[100]; // Adjust size as needed for your application

int main(int argc, char *argv[]) {
	int ret;
	int outputCount = 0;
	IMPFrameInfo *frame;
	//IMPFSChnAttr fs_chn_attr[2];
	FILE *fp;
	
	//read configs
	if (parse_ini_file("config.ini") == 0) {
        print_config(&config);
		initialize_chn();
		printf("chn initialised\n");
    }
	//Necessary for lib_imp commands
	IMP_System_GetCPUInfo(); 

	ret = start_camera();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):Failed setting up camera\n", __func__, __LINE__);
		return -1;
	}
    // ApplyCameraSettings(&cfg, "natural");
    ApplyCameraSettings(&config, "qrcode");

	int validQRFound = 0;
	//uint32_t y_size = 0;

	while (validQRFound == 0) {
		ret = IMP_FrameSource_GetFrame(0, &frame);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_GetFrame failed\n", __func__, __LINE__);
			stop_camera(); // Attempt to stop camera before exiting
			return -1;
		}
		// For Y channel
		//y_size = frame->width * frame->height;
		struct QROutput *outputs = scan_qr_with_quirc((uint8_t*)frame->virAddr, frame->width, frame->height, &outputCount);

		for (int i = 0; i < outputCount; ++i) {
			if (outputs[i].data != NULL) {
				printf("QR Code %d: %s\n", i, outputs[i].data);
				validQRFound = 1; // A valid QR code is found
			}
		}
		
		for (int i = 0; i < outputCount; ++i) {
			free(outputs[i].data); // Free dynamically allocated data field for all outputs
		}
		free(outputs); // Prevent memory leaks
		
		ret = IMP_FrameSource_ReleaseFrame(0, frame);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_ReleaseFrame failed\n", __func__, __LINE__);
			stop_camera(); // Attempt to stop camera before exiting
			return -1;
		}
	}

    if (validQRFound) {
        // Save the frame containing the valid QR code
		fp = fopen("/tmp/snap.yuv", "wb");
		if(fp == NULL) {
			IMP_LOG_ERR(TAG, "%s(%d):open error !\n", __func__, __LINE__);
			return -1;
		}

        fwrite((void *)frame->virAddr, 1, frame->size, fp);
		printf("Saved frame dimensions: %dx%d\n", frame->width, frame->height);
        fclose(fp); // Close the file after writing
        fp = NULL; // Mark the file pointer as handled
    }


	ret = stop_camera();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):Failed stopping camera\n", __func__, __LINE__);
		return -1;
	}
	return 0; // Successfully processed
}