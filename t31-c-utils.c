#include <stdlib.h>
#include <imp/imp_log.h>
#include <stdio.h>
#include <quirc/quirc.h>
#include <string.h>

#include "t31-c-utils.h"
#include "common.h"

#define TAG "T31-C-UTILS"
extern struct chn_conf chn[];

void save_image_to_pgm(const uint8_t *imageData, int width, int height, const char *filename) {
    FILE *file = fopen(filename, "wb"); // Open file for writing in binary mode
    if (!file) {
        perror("Failed to open file for writing");
        return;
    }

    // Write the PGM header
    fprintf(file, "P5\n%d %d\n255\n", width, height);

    // Write the image data
    size_t written = fwrite(imageData, 1, width * height, file);
    if (written != (size_t)(width * height)) {
        perror("Failed to write complete image data to file");
    }

    fclose(file);
}


/* Function to decode QR codes from a grayscale image */
struct QROutput *scan_qr_with_quirc(uint8_t *imageData, int width, int height, int *outputCount) {
    struct quirc *qr = quirc_new();
    if (!qr) {
        perror("Failed to allocate memory for quirc");
        return NULL;
    }

    if (quirc_resize(qr, width, height) < 0) {
        perror("Failed to allocate video memory");
        quirc_destroy(qr);
        return NULL;
    }

    uint8_t *image = quirc_begin(qr, NULL, NULL);
    memcpy(image, imageData, width * height);
    quirc_end(qr);

    int count = quirc_count(qr);
    *outputCount = count;

    //struct QROutput *outputs = malloc(sizeof(struct QROutput) * count);
    struct QROutput *outputs = (struct QROutput*)malloc(sizeof(struct QROutput) * count);

    if (!outputs) {
        perror("Failed to allocate memory for outputs");
        quirc_destroy(qr);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        struct quirc_code qcode; // Define quirc_code here
        struct quirc_data qdata; // Define quirc_data here
        
        quirc_extract(qr, i, &qcode);

        if (quirc_decode(&qcode, &qdata) == QUIRC_SUCCESS) {
            outputs[i].data = strdup((char *)qdata.payload);
        } else {
            outputs[i].data = NULL; // Indicate failure
        }
    }

    quirc_destroy(qr);
    return outputs;
}

void print_histogram_ascii(int* histogram, int max_height, int max_pixel_value) {
    // Find the maximum count to scale the histogram visually
    int max_count = 0;
    for (int i = 0; i < max_pixel_value; ++i) {
        if (histogram[i] > max_count) {
            max_count = histogram[i];
        }
    }

    // Calculate the scaling factor
    double scale = (double)max_count / max_height;

    // Print each bin as a line of '#' characters
    for (int i = 0; i < max_pixel_value; ++i) {
        printf("%3d: ", i); // Print the bin number
        
        int height = (int)(histogram[i] / scale);
        for (int j = 0; j < height; ++j) {
            printf("#");
        }
        
        printf("\n");
    }
}


void calculate_histogram(const unsigned char* y_channel, int* histogram, int num_pixels) {
    for (int i = 0; i < num_pixels; ++i) {
        histogram[y_channel[i]]++;
    }
}

void SetImp(const char* isp, int value) {
    char command[256]; // Buffer to hold the constructed command string

    // Format the command string with the isp parameter and integer value
    snprintf(command, sizeof(command), "/usr/sbin/imp-control %s %d", isp, value);
    printf("executing command: <%s> \n", command);
    printf("\n");

    // Execute the command
    int code = system(command);
    if (code != 0) {
        // Handle error
        printf("Failed to execute command: %s\n", command);
    }
}

int start_camera(int channel) {
   	IMPFSChnAttr fs_chn_attr[2];

    /* Step.1 System init */
	int ret = sample_system_init();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_System_Init() failed\n");
		return -1;
	}

	/* Step.2 FrameSource init */
	if (chn[channel].enable) {
		ret = IMP_FrameSource_CreateChn(chn[channel].index, &chn[channel].fs_chn_attr);
		if(ret < 0){
			IMP_LOG_ERR(TAG, "IMP_FrameSource_CreateChn(chn%d) error !\n", chn[channel].index);
			return -1;
		}

		ret = IMP_FrameSource_SetChnAttr(chn[channel].index, &chn[channel].fs_chn_attr);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_SetChnAttr(chn%d) error !\n",  chn[channel].index);
			return -1;
		}
	}

	/* Step.3 Snap raw config */
	ret = IMP_FrameSource_GetChnAttr(channel, &fs_chn_attr[channel]);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_GetChnAttr failed\n", __func__, __LINE__);
		return -1;
	}
#if 0
	ret = IMP_ISP_Tuning_SetISPBypass(IMPISP_TUNING_OPS_MODE_DISABLE);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_ISP_Tuning_SetISPBpass failed\n", __func__, __LINE__);
		return -1;
	}
#endif
	fs_chn_attr[channel].pixFmt = PIX_FMT_NV12;//PIX_FMT_YUYV422;
	ret = IMP_FrameSource_SetChnAttr(channel, &fs_chn_attr[channel]);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_SetChnAttr failed\n", __func__, __LINE__);
		return -1;
	}

	/* Step.3 config sensor reg to output colrbar raw data*/
	/* to do */

	/* Step.4 Stream On */
	if (chn[channel].enable){
		ret = IMP_FrameSource_EnableChn(chn[channel].index);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_EnableChn(%d) error: %d\n", ret, chn[channel].index);
			return -1;
		}
	}

	/* Step.4 Snap raw */
	ret = IMP_FrameSource_SetFrameDepth(channel, 1);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_SetFrameDepth failed\n", __func__, __LINE__);
		return -1;
	}
    return 0;
}

int stop_camera(int channel) {
    int ret = IMP_FrameSource_SetFrameDepth(channel, 0);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "%s(%d):IMP_FrameSource_SetFrameDepth failed\n", __func__, __LINE__);
		return -1;
	}
	/* end */

#if 0
	ret = IMP_ISP_Tuning_SetISPBypass(IMPISP_TUNING_OPS_MODE_ENABLE);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "error:(%s,%d),IMP_ISP_Tuning_SetISPBypass failed.\n",__func__,__LINE__);
		return -1;

	}
#endif
	/* Step.5 Stream Off */
	if (chn[channel].enable){
		ret = IMP_FrameSource_DisableChn(chn[channel].index);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_DisableChn(%d) error: %d\n", ret, chn[channel].index);
			return -1;
		}
	}

	/* Step.6 FrameSource exit */
	if (chn[channel].enable) {
		/*Destroy channel i*/
		ret = IMP_FrameSource_DestroyChn(channel);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_FrameSource_DestroyChn() error: %d\n", ret);
			return -1;
		}
	}

	/* Step.7 recover sensor reg to output normal image*/
	/* to do */

	/* Step.8 System exit */
	ret = sample_system_exit();
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "sample_system_exit() failed\n");
		return -1;
	}
    return 0;
}