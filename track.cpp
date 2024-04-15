#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>
#include <iostream>
#include <vector>

int main() {
    // Open the video file
    cv::VideoCapture capture("random_falling_640.mp4");
    if (!capture.isOpened()) {
        std::cerr << "Error opening video file" << std::endl;
        return -1;
    }

    // Create the KNN background subtractor
    cv::Ptr<cv::BackgroundSubtractorKNN> knn = cv::createBackgroundSubtractorKNN();
    knn->setDetectShadows(false);
    knn->setDist2Threshold(400);
    knn->setkNNSamples(2);
    knn->setHistory(500);

    cv::Mat frame, fgMask;
    double learningRate = 0.04;
    int whitePixelCount = 0;
    bool motionStopped = false;
    cv::Point lastWhitePixel(-1, -1);
    int64 lastStopTime = 0; // To track when motion last stopped
    double refractoryPeriod = 5.0; // 5 seconds refractory period
    int frameNumber = 0; // Initialize frame number counter
    int64 startTick, endTick; // For timing
    double duration; // Duration of frame processing

    while (true) {
        capture >> frame;
        if (frame.empty()) break; // Break if no more frames are available

        frameNumber++; // Increment frame counter
        startTick = cv::getTickCount(); // Start time

        // Apply KNN background subtraction
        knn->apply(frame, fgMask, learningRate);

        // Refine the mask
        cv::erode(fgMask, fgMask, cv::Mat(), cv::Point(-1, -1), 2);
        cv::dilate(fgMask, fgMask, cv::Mat(), cv::Point(-1, -1), 2);

        // Count white pixels
        whitePixelCount = cv::countNonZero(fgMask);
        double currentTime = static_cast<double>(cv::getTickCount()) / cv::getTickFrequency();
        
        // Tracking motion and last white pixel location
        if (whitePixelCount > 0) {
            for (int y = 0; y < fgMask.rows; y++) {
                for (int x = 0; x < fgMask.cols; x++) {
                    if (fgMask.at<uchar>(y, x) > 0) {
                        lastWhitePixel = cv::Point(x, y);
                    }
                }
            }
            motionStopped = false;
        } else if (whitePixelCount == 0 && currentTime - lastStopTime > refractoryPeriod && !motionStopped) {
            motionStopped = true;
            lastStopTime = currentTime;
            std::cout << "Motion stopped at frame " << frameNumber << ". Last white pixel at: " << lastWhitePixel << std::endl;
        }

        endTick = cv::getTickCount(); // End time
        duration = (endTick - startTick) / cv::getTickFrequency(); // Calculate duration
        std::cout << "Frame " << frameNumber << " processed in " << duration << " seconds." << std::endl;

        // No video saving or display
        if (char key = (char)cv::waitKey(30); key == 'q' || key == 27) {
            break; // Exit on 'q' or 'ESC'
        }
    }

    // Cleanup
    capture.release();
    return 0;
}
