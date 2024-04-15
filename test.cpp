#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture capture("random_falling_640.mp4");
    if (!capture.isOpened()) {
        std::cout << "Failed to open video file." << std::endl;
        return -1;
    }

    cv::Mat frame;
    while (true) {
        capture >> frame;
        if (frame.empty())
            break;

        // Process the frame here

        cv::imshow("Frame", frame);

        if (cv::waitKey(30) == 27)  // Press 'Esc' to exit
            break;
    }

    capture.release();
    cv::destroyAllWindows();

    return 0;
}