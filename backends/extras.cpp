#include "backend.hpp"
#include <opencv2/opencv.hpp>
#include <iostream> // Ensure iostream is included for std::cout and std::cerr

namespace sf {
    int width = 640;
    int height = 360;
    // Function to resize the image if it's not correct
    cv::Mat ensureResolution(cv::Mat frame) {
        // Check if the image resolution is not correct
        if (frame.cols != width || frame.rows != height) {
            // Resize the image to desired size
            cv::resize(frame, frame, cv::Size(width, height));
        }
        return frame;
    }

    cv::Mat getFrame() {
        std::string imagePath = "/tmp/snapshot.jpg";
        cv::Mat frame = cv::imread(imagePath, cv::IMREAD_COLOR);

        if(frame.empty()) {
            std::cerr << "Error: Unable to load image from " << imagePath << std::endl;
            return frame; // Return the empty frame
        }

        // Ensure the image is as desired
        frame = ensureResolution(frame);

        return frame;
    }

    void SetupGPIO() {
        std::cout << "Setup GPIO" << std::endl;
    }

    void SetLEDColour(LEDColour colour) {
        std::cout << "Set LED Colour: " << static_cast<int>(colour) << std::endl; // Assuming LEDColour is an enum
    }

    void TestLEDs(int count = 3) {
        for (int i = 0; i < count; i++) {
            std::cout << "Test LED: " << i << std::endl;
        }
    }

    void SetupWifi(std::string ssid, std::string passkey) {
        std::cout << "Wifi: " << ssid << " | " << passkey << std::endl;
    }

    void SetAutoExposure(int value) {
        std::cout << "Set Auto Exposure: " << value << std::endl;
    }
}
