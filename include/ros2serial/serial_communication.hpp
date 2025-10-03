#pragma once

#include <iostream>
#include <chrono>
#include <memory>
#include <string>
#include <functional>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <regex>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class SerialCommunication
{
public:
    json receive_data = {
        {"encoder_left", 0.00},
        {"encoder_right", 0.00}
    };
    json send_data = {
        {"linear_x", 0.00},
        {"angular_z", 0.00}
    };
public:
    SerialCommunication(std::string serial_port, int baudrate);
    virtual ~SerialCommunication();

    string receiveSerialData(const string& init_object);
    json decodeSerialData(const string& serial_data, const string& init_data);
    void sendSerialData(const string& message);

private:
    int fd_;

    void openSerial(const char *device_name, int baudrate);
};