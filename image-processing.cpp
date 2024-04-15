#include "image-processing.h"
#include <imp/imp_framesource.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

const char* PixelFormatToString(IMPPixelFormat format) {
    switch (format) {
        case PIX_FMT_YUV420P: return "PIX_FMT_YUV420P";
        case PIX_FMT_YUYV422: return "PIX_FMT_YUYV422";
        case PIX_FMT_UYVY422: return "PIX_FMT_UYVY422";
        case PIX_FMT_NV12: return "PIX_FMT_NV12";
        // Add cases for all enum values...
        case PIX_FMT_BGR24: return "PIX_FMT_BGR24";
        case PIX_FMT_RGB24: return "PIX_FMT_RGB24";
        // ...and so on for each enum value
        default: return "Unknown Format";
    }
}

int process_frame(void* frameData, IMPFSChnAttr fsChnAttr) {
    static int counter = 0;
    int width = fsChnAttr.picWidth;
    int height = fsChnAttr.picHeight;
    auto start = std::chrono::high_resolution_clock::now();
    if (counter == 0) {
        printf("Pixel Format: %s\n", PixelFormatToString(fsChnAttr.pixFmt));
        printf("Width: %d Height %d\n", width, height);
        fflush(stdout);
    }
    cv::Mat picBGR;

    if (fsChnAttr.pixFmt == PIX_FMT_NV12) {
        // cv::Mat yMat(height, width, CV_8UC1, frameData);
        // cv::Mat uvMat(height / 2, width / 2, CV_8UC2, frameData + width * height);
        cv::Mat picYV12 = cv::Mat(height * 3/2, width, CV_8UC1, frameData);
        cv::cvtColor(picYV12, picBGR, cv::COLOR_YUV2BGR_YV12);
    }
    else {
        std::cerr << "Unsupported format" << std::endl;
        return -1; // Consider returning an error code or handling error differently
    }

    // Ensure frameMat is valid before proceeding
    if (!picBGR.empty()) {
        // Process frameMat
        // cv::Mat grayMat;
        // cv::cvtColor(picBGR, grayMat, cv::COLOR_BGR2GRAY);
        // Further processing...
        // Display original image size
        if (counter == 0) std::cout << "Original size: " << picBGR.size() << std::endl;
            
        // Reduce the image size by 3 in  each dimension
        // cv::resize(picBGR, smallBGR, cv::Size(picBGR.cols / 3, picBGR.rows / 3), 0, 0, cv::INTER_LINEAR);

        // Display reduced image size
        // if (counter == 0) std::cout << "Reduced size: " << smallBGR.size() << std::endl;

        std::cout << "Processing frame: " << ++counter << std::endl;
        // Stop timing
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate the elapsed time in milliseconds
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

    } else {
        std::cerr << "Frame processing skipped due to unsupported format or other issues." << std::endl;
    }
    return 0;
}

int process_frame_older(void* frameData, IMPFSChnAttr fsChnAttr) {
    printf("Doing frame\n");
    //static int counter = 0;
    int width = fsChnAttr.picWidth;
    int height = fsChnAttr.picHeight;
    //auto start = std::chrono::high_resolution_clock::now();
    //if (counter == 0) {
        printf("Pixel Format: %s\n", PixelFormatToString(fsChnAttr.pixFmt));
        printf("Width: %d Height %d\n", width, height);
    //    fflush(stdout);
    // }
    //cv::Mat picBGR;
    //cv::Mat smallBGR;

    /*if (fsChnAttr.pixFmt == PIX_FMT_NV12) {
        // cv::Mat yMat(height, width, CV_8UC1, frameData);
        // cv::Mat uvMat(height / 2, width / 2, CV_8UC2, frameData + width * height);
        cv::Mat picYV12 = cv::Mat(height * 3/2, width, CV_8UC1, frameData);
        cv::cvtColor(picYV12, picBGR, cv::COLOR_YUV2BGR_YV12);
    }
    else if (fsChnAttr.pixFmt == PIX_FMT_RGB24) {
        cv::Mat picBGR = cv::Mat(height, width, CV_8UC3, frameData);
    }
    else {
        std::cerr << "Unsupported format" << std::endl;
        return -1; // Consider returning an error code or handling error differently
    }

    // Ensure frameMat is valid before proceeding
    if (!picBGR.empty()) {
        // Process frameMat
        // cv::Mat grayMat;
        // cv::cvtColor(picBGR, grayMat, cv::COLOR_BGR2GRAY);
        // Further processing...
        // Display original image size
        if (counter == 0) std::cout << "Original size: " << picBGR.size() << std::endl;
            
        // Reduce the image size by 3 in  each dimension
        // cv::resize(picBGR, smallBGR, cv::Size(picBGR.cols / 3, picBGR.rows / 3), 0, 0, cv::INTER_LINEAR);

        // Display reduced image size
        // if (counter == 0) std::cout << "Reduced size: " << smallBGR.size() << std::endl;

        std::cout << "Processing frame: " << ++counter << std::endl;
        // Stop timing
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate the elapsed time in milliseconds
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "Execution time: " << duration.count() << " milliseconds" << std::endl;

    } else {
        std::cerr << "Frame processing skipped due to unsupported format or other issues." << std::endl;
    }*/
    return 0;
}

