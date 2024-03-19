#include "backend.hpp"

cv::Mat sf::getFrame()
{
    cv::Mat frame;


    return frame;
}

void sf::SetupGPIO() {
    // enable GPIO
    system("echo 38 > /sys/class/gpio/export");
    system("echo 39 > /sys/class/gpio/export");

    // set as output
    system("echo out > /sys/class/gpio/gpio38/direction");
    system("echo out > /sys/class/gpio/gpio39/direction");

    // set defaultly off
    system("echo 1 > /sys/class/gpio/gpio38/value");
    system("echo 1 > /sys/class/gpio/gpio39/value");


    TestLEDs(3);
}

void sf::SetLEDColour(LEDColour colour)
{

    switch (colour)
    {
    case LEDColour::PRIMARY:
        system("echo 1 > /sys/class/gpio/gpio38/value");
        system("echo 0 > /sys/class/gpio/gpio39/value");

        break;
    case LEDColour::ERROR:
        system("echo 0 > /sys/class/gpio/gpio38/value");
        system("echo 1 > /sys/class/gpio/gpio39/value");

        break;
    case LEDColour::SUCCESS:
        system("echo 0 > /sys/class/gpio/gpio38/value");
        system("echo 0 > /sys/class/gpio/gpio39/value");
        break;
    case LEDColour::OFF:
        system("echo 1 > /sys/class/gpio/gpio38/value");
        system("echo 1 > /sys/class/gpio/gpio39/value");

        break;

    default:
        std::cout << "SetLEDColour '" << colour << "' not implemented" << std::endl;
        break;
    }
}

void sf::TestLEDs(int count)
{
    for (int i = 0; i < count; i++) {
        SetLEDColour(LEDColour::PRIMARY);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
        SetLEDColour(LEDColour::ERROR);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
        SetLEDColour(LEDColour::SUCCESS);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
        SetLEDColour(LEDColour::OFF);
        std::this_thread::sleep_until(std::chrono::system_clock::now() + std::chrono::seconds(1));
    }
}

void sf::SetupWifi(std::string ssid, std::string passkey) {
    std::string cmd = "wifi.sh " + ssid + " " + passkey;
    system(cmd.c_str());
}

void sf::SetAutoExposure(int value) {
    int code = system("/usr/sbin/imp-control aecomp " + value);
    if (code != 0) {
        //TODO Error
    }
}