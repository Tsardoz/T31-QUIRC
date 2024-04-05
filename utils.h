#ifndef UTILS_H
#define UTILS_H

#include <stdint.h> // Include stdint.h for uint8_t definition
#include <quirc.h> // Ensure quirc.h or the path to quirc.h is included if it defines quirc_point

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char *data;
    struct quirc_point corners[4];
} QROutput;

QROutput* scan_qr_with_quirc(uint8_t* imageData, int width, int height, int* outputCount);

void SetImp(const char* isp, int value);
int start_camera();
int stop_camera();
void calculate_histogram(const unsigned char* y_channel, int* histogram, int num_pixels);
void print_histogram_ascii(int* histogram, int max_height, int max_pixel_value);

#ifdef __cplusplus
}
#endif

#endif // UTILS_H
