#include <stdlib.h>
#include <stdio.h>
#include <quirc.h>
#include <string.h>
#include "utils.h" // Make sure this file defines the updated QROutput struct

/* Function to decode QR codes from a grayscale image */
QROutput *scan_qr_with_quirc(uint8_t *imageData, int width, int height, int *outputCount) {
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

    QROutput *outputs = (QROutput *)malloc(sizeof(QROutput) * count);
    if (!outputs) {
        perror("Failed to allocate memory for outputs");
        quirc_destroy(qr);
        return NULL;
    }

    for (int i = 0; i < count; i++) {
        struct quirc_code qcode;
        struct quirc_data qdata;
        
        quirc_extract(qr, i, &qcode);
        if (quirc_decode(&qcode, &qdata) == QUIRC_SUCCESS) {
            outputs[i].data = strdup((char *)qdata.payload);
            memcpy(outputs[i].corners, qcode.corners, sizeof(qcode.corners));
        } else {
            outputs[i].data = NULL;
            memset(outputs[i].corners, 0, sizeof(outputs[i].corners)); // Zero out corners if decode fails
        }
    }

    quirc_destroy(qr);
    return outputs;
}
