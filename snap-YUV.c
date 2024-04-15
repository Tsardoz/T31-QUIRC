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

#include "image-processing.h"
#include "common.h"
#include "t31-c-utils.h"
#include "parse_ini.h"
#define TAG "Snap-YUV"

//channels for low and high quality images
#define LOWRES 0
#define HIGHRES 1
//state
#define QRCODE 0
#define MOTION 1
#define SNAPSHOT 2

extern struct chn_conf chn[];
SensorConfig config;

INIEntry entries[100]; // Adjust size as needed for your application

//int start_camera(int channel);
//int stop_camera(int channel);
int find_qr(void);
int find_motion(void);
//void initialize_chn(int channel);

void ApplyCameraSettings(const SensorConfig* cfg, int state) {
    if (cfg == NULL) {
        printf("Invalid configuration.\n");
        return;
    }
    
    const ImageProfile* profile = NULL;
    
    if ((state==SNAPSHOT) || (state == MOTION)) {
        profile = &cfg->natural;
    } else if (state == QRCODE) {
        profile = &cfg->qrcode;
    } else {
        printf("Unknown state: %d\n", state);
        return;
    }

    SetImp("aecomp", profile->aecomp);
    SetImp("contrast", profile->contrast);
    SetImp("brightness", profile->brightness);
    SetImp("sharpness", profile->sharpness);
    SetImp("drc", profile->drc);
}

int find_motion() {
    int ret;
    int ctr = 0;
    IMPFrameInfo *frame;

    printf("finding motion\n");
    for (int i=0;i<5;i++) {
        ret = IMP_FrameSource_GetFrame(0, &frame);
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_GetFrame failed\n", __func__, __LINE__);
            printf("IMP_FrameSource_GetFrame failed\n");
            stop_camera(LOWRES);
            return -1;
        }

        if (frame == NULL || frame->virAddr == NULL) {
            IMP_LOG_ERR(TAG, "%s(%d): Invalid frame data\n", __func__, __LINE__);
            return -1;
        }
        ret = process_frame((void *)frame->virAddr, chn[0].fs_chn_attr);
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "%s(%d):Processing frame failed\n", __func__, __LINE__);
            return -1;
        }
        ret = IMP_FrameSource_ReleaseFrame(0, frame);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_ReleaseFrame failed\n", __func__, __LINE__);
			stop_camera(QRCODE); // Attempt to stop camera before exiting
			return -1;
		}
    }
    return 0;
}


int find_qr() {
	int ret;
	int outputCount = 0;
	IMPFrameInfo *frame;
	//IMPFSChnAttr fs_chn_attr[2];
	FILE *fp;

	int validQRFound = 0;
	//uint32_t y_size = 0;
	printf("finding QR\n");
	while (validQRFound == 0) {
		ret = IMP_FrameSource_GetFrame(0, &frame);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_GetFrame failed\n", __func__, __LINE__);
			stop_camera(LOWRES); // Attempt to stop camera before exiting
			return -1;
		}
		// For Y channel
		//y_size = frame->width * frame->height;
		struct QROutput *outputs = scan_qr_with_quirc((uint8_t*)frame->virAddr, frame->width, frame->height, &outputCount);
		//printf("Len outputs %d\n",outputCount);
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
			stop_camera(QRCODE); // Attempt to stop camera before exiting
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
	return 0;
}

int main(int argc, char *argv[]) {
    int state = QRCODE;
    //int state = MOTION;
    int channel;
    int ret;

    //state = QRCODE;

    if ((state == QRCODE) || (state == MOTION)) {
        channel = LOWRES; //qrcode low res
    } else {
        channel = HIGHRES; //natural high res
    }

    //read configs
    if (parse_ini_file("config.ini") == 0) {
        print_config(&config);
        initialize_chn(channel);
        printf("chn %d initialised\n", channel);
    }
    // Necessary for lib_imp commands
    IMP_System_GetCPUInfo(); 
    ret = start_camera(channel);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "%s(%d):Failed setting up camera\n", __func__, __LINE__);
        return -1;
    }

    if (state == QRCODE) {
        ApplyCameraSettings(&config, QRCODE);
        ret = find_qr();
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "Failed finding QR code\n");
            return -1;
        }
    } else if (state == MOTION) {
        ApplyCameraSettings(&config, MOTION);
        ret = find_motion();
        if (ret < 0) {
            IMP_LOG_ERR(TAG, "Failed finding motion\n");
            return -1;
        }
    }

    ret = stop_camera(channel);
    if (ret < 0) {
        IMP_LOG_ERR(TAG, "%s(%d):Failed stopping camera\n", __func__, __LINE__);
        return -1;
    }
    return 0; // Successfully processed
}
