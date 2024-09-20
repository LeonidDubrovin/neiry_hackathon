#pragma once

//#include <arpa/inet.h>
#include <WinSock2.h>
#include <sys/types.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
//#include <sys/socket.h>
//#include <unistd.h>
#include <vector>

//#include "CppPythonSocketConfig.h"  // To check version, USE_OPENCV...


namespace socket_communication {

struct Data {
    float fatigueScore;
    float gravityScore;
    float concentrationScore;
    float accumulatedFatigue;
    float individualPeakFrequency;
};


class Client
{
public:
    Client();

    Client(const std::string& ip, int port);

    ~Client();

    void Close();

    void Init(const std::string& ip, int port);

    void SendData(const Data& data);

private:
    int client_;
    static constexpr int size_message_length_ = 1024;  // Buffer size for the length
};

} // namespace socket_communication
