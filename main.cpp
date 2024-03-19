#include <iostream>
#include <thread>
#include <signal.h>

#include "nlohmann/json.hpp"
#include "backends/backend.hpp"
#include "qr.hpp"

// #include "tracker.hpp"

enum State
{
    QR_SCANNING,
    TRACKING,
};

// struct DeviceSettings
// {
//     std::string id;
//     std::string key; // Unsure how to handle this yet
//     DeviceState state;
//     std::string url;
// };

void exitHandler(sig_atomic_t s)
{
    printf("Caught signal %d\n", s);
    exit(1);
}

// void setup(DeviceSettings *settings)
// {
//     // Load variables
//     const char *idEnv = std::getenv("DEVICE");
//     if (idEnv == nullptr)
//     {
//         return;
//     }
//     settings->id = idEnv;
//     settings->key = "b";
//     settings->url = "c";

//     // Check Internet Connection - ping backend?
//     settings->state = QR_SCANNING;
// }

// logcat -f /overlay/root/output.log -r 32

// Entry Point
int main()
{
    signal(SIGINT, exitHandler);

    sf::SetupGPIO();

    State state = State::QR_SCANNING;

    while (true)
    {
        cv::Mat frame = sf::getFrame();

        switch (state)
        {
        case State::QR_SCANNING:
        {
            std::vector<QROutput> outputs = scanQRWithQuirc(frame);

            for (int i = 0; i < outputs.size(); i++)
            {
                nlohmann::json qrData = nlohmann::json::parse(outputs[i].data.payload, nullptr, false);
                if (qrData.is_discarded())
                {
                    //Could not pass qr data as JSON
                    // TODO LOG
                    continue;
                }

                if (!(qrData.contains("u") &&
                      qrData.contains("e") &&
                      qrData.contains("w") &&
                      qrData.contains("p")))
                {
                    // Data does not contain correct information
                    // TODO LOG

                    continue;
                }

                std::cout << qrData.dump() << std::endl;
            }

            break;
        }
        case State::TRACKING:

            break;

        default:
            break;
        }
    }

    return 1;
}

void cleanup()
{
}