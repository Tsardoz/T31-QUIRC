#ifndef T31CUTILS_H
#define T31CUTILS_H

#include <stdint.h> 

#ifdef __cplusplus
extern "C" {
#endif

struct QROutput {
    char *data; /* Dynamically allocate memory for decoded text */
};

struct QROutput *scan_qr_with_quirc(uint8_t *imageData, int width, int height, int *outputCount);
void SetImp(const char* isp, int value);
int start_camera(int channel);
int stop_camera(int channel);
void calculate_histogram(const unsigned char* y_channel, int* histogram, int num_pixels);
void print_histogram_ascii(int* histogram, int max_height, int max_pixel_value);

typedef struct SettingNode {
    char name[100];  // Name of the setting
    uint8_t value;       // Value of the setting
    struct SettingNode* next;  // Pointer to the next node
} SettingNode;

#define MAX_SECTION_SIZE 50
#define MAX_KEY_SIZE 50
#define MAX_VALUE_SIZE 100

typedef struct {
    char section[MAX_SECTION_SIZE];
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
} INIEntry;

// Head of the list
extern char sensor_type[100]; 
extern int width;
extern int height;

#ifdef __cplusplus
}
#endif

#endif // T31CUTILS_H