#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // For isspace and tolower functions
#include "parse_ini.h"
#include "t31-c-utils.h"

char* trim_whitespace(char* str) {
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

void ini_parse_line(char *section, char *key, char *value) {
    // Handle sensor section
    if (strcmp(section, "sensor") == 0) {
        if (strcmp(key, "name") == 0) {
            strncpy(config.name, value, MAX_STR-1);
        } else if (strcmp(key, "width") == 0) {
            config.width = atoi(value);
        } else if (strcmp(key, "height") == 0) {
            config.height = atoi(value);
        } else if (strcmp(key, "i2c_addr") == 0) {
            config.i2c_addr = strtol(value, NULL, 16);
        }
    }
    // Handle channel sections
    else if (strcmp(section, "channel0") == 0 || strcmp(section, "channel1") == 0) {
        int channelIndex = section[7] - '0';
        if (strcmp(key, "width") == 0) {
            config.channels[channelIndex].width = atoi(value);
        } else if (strcmp(key, "height") == 0) {
            config.channels[channelIndex].height = atoi(value);
        } else if (strcmp(key, "frnum") == 0) {
            config.channels[channelIndex].frnum = atoi(value);
        } else if (strcmp(key, "frden") == 0) {
            config.channels[channelIndex].frden = atoi(value);
        }
    }
    // Existing logic for qrcode and natural sections
    else if (strcmp(section, "qrcode") == 0 || strcmp(section, "natural") == 0) {
        ImageProfile *profile = (strcmp(section, "qrcode") == 0) ? &config.qrcode : &config.natural;
        if (strcmp(key, "aecomp") == 0) {
            profile->aecomp = atoi(value);
        } else if (strcmp(key, "contrast") == 0) {
            profile->contrast = atoi(value);
        } else if (strcmp(key, "brightness") == 0) {
            profile->brightness = atoi(value);
        } else if (strcmp(key, "sharpness") == 0) {
            profile->sharpness = atoi(value);
        } else if (strcmp(key, "drc") == 0) {
            profile->drc = atoi(value);
        }
    }
}

int parse_ini_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Unable to open the file");
        return -1;
    }

    char line[256];
    char section[MAX_STR] = {0};

    while (fgets(line, sizeof(line), file)) {
        char *trimmedLine = trim_whitespace(line);
        if (trimmedLine[0] == ';' || trimmedLine[0] == '#' || trimmedLine[0] == '\n') {
            continue; // Skip comments and empty lines
        }
        if (trimmedLine[0] == '[') {
            sscanf(trimmedLine, "[%[^]]]", section); // Get section name
            section[strcspn(section, "\r\n")] = 0; // Remove newline characters
            continue;
        }

        char key[MAX_STR], value[MAX_STR];
        if (sscanf(trimmedLine, "%[^=]=%s", key, value) == 2) {
            char *trimmedKey = trim_whitespace(key);
            char *trimmedValue = trim_whitespace(value);
            ini_parse_line(section, trimmedKey, trimmedValue);
        }
    }

    fclose(file);
    return 0;
}


void print_config(const SensorConfig *cfg) {
    printf("Sensor Name: %s\n", cfg->name);
    printf("Sensor i2c_addr: 0x%X\n", cfg->i2c_addr);
    printf("Sensor Width: %d\n", cfg->width);
    printf("Sensor Height: %d\n", cfg->height);
    for (int i = 0; i < MAX_CHANNELS; ++i) {
        printf("Channel %d Width: %d\n", i, cfg->channels[i].width);
        printf("Channel %d Height: %d\n", i, cfg->channels[i].height);
        printf("Channel %d Frame Rate Num: %d\n", i, cfg->channels[i].frnum);
        printf("Channel %d Frame Rate Den: %d\n", i, cfg->channels[i].frden);
    }

    // Add qrcode and natural profile printing
    printf("QR Code Profile AEComp: %d\n", cfg->qrcode.aecomp);
    printf("QR Code Profile Contrast: %d\n", cfg->qrcode.contrast);
    printf("QR Code Profile Brightness: %d\n", cfg->qrcode.brightness);
    printf("QR Code Profile Sharpness: %d\n", cfg->qrcode.sharpness);
    printf("QR Code Profile DRC: %d\n", cfg->qrcode.drc);

    printf("Natural Profile AEComp: %d\n", cfg->natural.aecomp);
    printf("Natural Profile Contrast: %d\n", cfg->natural.contrast);
    printf("Natural Profile Brightness: %d\n", cfg->natural.brightness);
    printf("Natural Profile Sharpness: %d\n", cfg->natural.sharpness);
    printf("Natural Profile DRC: %d\n", cfg->natural.drc);
}


/*int main() {
    if (parse_ini_file("config.ini") == 0) {
        print_config(&config);
    }
    return 0;
}*/
