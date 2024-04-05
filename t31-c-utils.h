// t31-c-utils.h

#ifndef T31CUTILS_H
#define T31CUTILS_H

#ifdef __cplusplus
extern "C" {
#endif

struct QROutput {
    char *data; /* Dynamically allocate memory for decoded text */
};

struct QROutput *scan_qr_with_quirc(uint8_t *imageData, int width, int height, int *outputCount);
void SetImp(const char* isp, int value);
int start_camera();
int stop_camera();
void calculate_histogram(const unsigned char* y_channel, int* histogram, int num_pixels);
void print_histogram_ascii(int* histogram, int max_height, int max_pixel_value);

#ifdef __cplusplus
}
#endif

#endif // T31CUTILS_H