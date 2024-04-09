#define MAX_CHANNELS 2
#define MAX_STR 100

typedef struct {
    int width;
    int height;
    int frnum;
    int frden;
} Channel;

typedef struct {
    int aecomp;
    int contrast;
    int brightness;
    int sharpness;
    int drc;
} ImageProfile;

typedef struct {
    char name[MAX_STR];
    int i2c_addr;
    int width;
    int height;
    Channel channels[MAX_CHANNELS];
    ImageProfile qrcode;
    ImageProfile natural;
} SensorConfig;

extern SensorConfig config;

void print_config(const SensorConfig *cfg);
int parse_ini_file(const char *file_path);
void ini_parse_line(char *section, char *key, char *value);
char* trim_whitespace(char* str);
void ApplyCameraSettings(const SensorConfig* cfg, const char* state);
