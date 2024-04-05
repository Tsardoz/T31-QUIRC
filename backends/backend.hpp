#pragma once

#include <string>
#include <iostream>
#include <thread>

#include <opencv2/core.hpp>
#include <imp/imp_framesource.h>

namespace sf
{
    // Define the dimensions as constexpr
    constexpr int FRAME_WIDTH = 640;  // Example width
    constexpr int FRAME_HEIGHT = 360; // Example height

    enum LEDColour
    {
        OFF = 0,
        PRIMARY = 1,
        SUCCESS = 2,
        ERROR = 3
    };

    //cv::Mat get_frame(IMPFSChnAttr fsChnAttr, int chn_num);
    int get_frame();
    cv::Mat get_jpg();

    void SetupGPIO();
    void SetLEDColour(LEDColour colour);
    void TestLEDs(int count);

    int SetupWifi(std::string ssid, std::string passkey);
    int CheckWifi();
    void SetAutoExposure(int value);
}
