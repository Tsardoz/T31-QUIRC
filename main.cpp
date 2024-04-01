#include <iostream>
#include <future>
#include <thread>
#include <signal.h>

#include "nlohmann/json.hpp"
#include "network.hpp"
#include "backends/backend.hpp"
#include "qr.hpp"

// #include "tracker.hpp"

enum State
{
    QR_SCANNING,
    TRACKING,
};

void exitHandler(sig_atomic_t s)
{
    printf("Caught signal %d\n", s);
    exit(1);
}

// logcat -f /overlay/root/output.log -r 32

// Entry Point
int main()
{
    signal(SIGINT, exitHandler);
    
    std::string url = "http://192.168.20.15:5173";
    // std::cout << settings->url << std::endl;

    std::string pingResponse = "";
    auto pingThread = std::async(std::launch::async, DoPingRequest, url, &pingResponse, true);

    sf::SetupGPIO();

    // Set to qr scaning mode in setup
    State state = State::QR_SCANNING;

    // If can ping router
    // If can ping backend
    int err = sf::CheckWifi();
    if (err == 0)
    {
        // Connected to wifi
        state = State::TRACKING;
    }

    while (true)
    {
        cv::Mat frame = sf::getFrame();

        switch (state)
        {
        case State::QR_SCANNING:
        {
            auto start = std::chrono::high_resolution_clock::now();

            std::vector<QROutput> outputs = scanQRWithQuirc(frame);

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            std::cout << "Took: " << duration.count() << " milliseconds" << std::endl;

            for (int i = 0; i < outputs.size(); i++)
            {
                std::cout << outputs[i].data.payload << std::endl;
                std::string payload((char *)outputs[i].data.payload);
                
                std::vector<std::pair<std::string, std::string>> keyValuePairs = parseKeyValuePairs(payload, ',');
                if (keyValuePairs.size() != 3) {
                    std::cout << "doesnt have 3 elements" << std::endl;
                }

                std::pair<std::string, std::string> id = keyValuePairs[0];
                std::pair<std::string, std::string> ssid = keyValuePairs[1];
                std::pair<std::string, std::string> passkey = keyValuePairs[2];

                if (id.first != "i" || ssid.first != "n" || passkey.first != "p") {
                    std::cout << "keys are not correct format" << std::endl;
                }

                int err = sf::SetupWifi(ssid.second, passkey.second);
                if (err == 0)
                {
                    // Connected to wifi
                    state = State::TRACKING;
                    std::string registerResponse = "";
                    std::string body = "{\"UserID\":\"" + id.second + "\"}";
                    bool success = DoRegisterRequest(url, body, &registerResponse);
                    break;
                }
            }

            break;
        }
        case State::TRACKING:

            break;

        default:
            break;
        }

        auto status = pingThread.wait_for(std::chrono::milliseconds(0));

        if (status == std::future_status::ready)
        {
            bool success = pingThread.get();
            if (success)
            {
                nlohmann::json resData = nlohmann::json::parse(pingResponse, nullptr, false);
                if (resData.is_discarded())
                {
                    std::cout << "cannot json response: " << pingResponse << std::endl;
                }
                else
                {
                    if (resData.contains("volume")) {
                        //Set volume
                        unsigned int volume = resData["volume"];
                        std::cout << volume << std::endl;
                    }
                }
            }
            pingResponse = "";
            pingThread = std::async(std::launch::async, DoPingRequest, url, &pingResponse, true);
        }
    }

    return 1;
}

void cleanup()
{
}