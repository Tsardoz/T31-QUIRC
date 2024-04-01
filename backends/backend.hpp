#pragma once

#include <string>
#include <iostream>
#include <thread>

#include <opencv2/core.hpp>

namespace sf
{
    enum LEDColour
    {
        OFF = 0,
        PRIMARY = 1,
        SUCCESS = 2,
        ERROR = 3
    };

    cv::Mat getFrame();

    void SetupGPIO();
    void SetLEDColour(LEDColour colour);
    void TestLEDs(int count);

    int SetupWifi(std::string ssid, std::string passkey);
    int CheckWifi();
    void SetAutoExposure(int value);
}
