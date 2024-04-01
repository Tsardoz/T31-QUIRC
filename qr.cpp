#include "qr.hpp"

std::vector<QROutput> scanQRWithQuirc(cv::Mat input)
{
    struct quirc *qr = quirc_new();
    if (!qr)
    {
        perror("Failed to allocate memory");
        abort();
    }
    if (quirc_resize(qr, 1920, 1080) < 0)
    {
        perror("Failed to allocate video memory");
        abort();
    }

    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    uint8_t *imageData = (uint8_t *)gray.data;

    sf::SetLEDColour(sf::LEDColour::PRIMARY);

    uint8_t *image = quirc_begin(qr, &gray.rows, &gray.cols);
    memcpy(image, imageData, gray.rows * gray.cols);
    quirc_end(qr);

    std::vector<QROutput> outputs;
    int numCodes = quirc_count(qr);
    for (int i = 0; i < numCodes; i++)
    {
        struct quirc_code code;
        struct quirc_data data;
        quirc_decode_error_t err;

        quirc_extract(qr, i, &code);

        err = quirc_decode(&code, &data);
        if (err == QUIRC_ERROR_DATA_ECC)
        {
            quirc_flip(&code);
            err = quirc_decode(&code, &data);
        }
        if (err)
        {
            continue;
        }
        else
        {
            QROutput output;
            output.code = code;
            output.data = data;
            outputs.push_back(output);
        }
    }

    quirc_destroy(qr);
    return outputs;
}

// Function to parse the input string into key-value pairs
std::vector<std::pair<std::string, std::string>> parseKeyValuePairs(const std::string& input, char delimiter) {
    std::vector<std::pair<std::string, std::string>> keyValuePairs;
    std::stringstream ss(input);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        std::string::size_type pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            keyValuePairs.push_back(std::make_pair(key, value));
        }
    }

    return keyValuePairs;
}