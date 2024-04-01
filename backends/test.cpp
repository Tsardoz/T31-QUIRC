#include "backend.hpp"


cv::Mat sf::getFrame() {
    cv::Mat frame;

    return frame;
}

void sf::SetupGPIO() {
    std::cout << "Setup GPIO" << std::endl;
}

void sf::SetLEDColour(LEDColour colour) {
    std::cout << "Set LED Colour: " << colour << std::endl;
}

void sf::TestLEDs(int count = 3) {
    for (int i = 0; i < count; i++)
    {
        std::cout << "Test LED: " << i << std::endl;
    }
}

int sf::SetupWifi(std::string ssid, std::string passkey) {
    std::cout << "Wifi: " << ssid << " | " << passkey << std::endl;
    return 0;
}

void sf::SetAutoExposure(int value) {
    std::cout << "Set Auto Exposure: " << value << std::endl;
}