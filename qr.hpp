#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <quirc.h>

#include "backends/backend.hpp"

struct QROutput
{
    quirc_code code;
    quirc_data data;
};

std::vector<QROutput> scanQRWithQuirc(cv::Mat input);

