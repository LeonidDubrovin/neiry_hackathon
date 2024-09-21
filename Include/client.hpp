#pragma once

//#include <arpa/inet.h>
#include <Winsock2.h>
#include <sys/types.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
//#include <sys/socket.h>
//#include <unistd.h>
#include <vector>


namespace socket_communication
{
namespace
{
    template<typename T>
    struct Value {
        T value;
        uint32_t code;
    };
}

struct Data {
    Value<float> fatigueScore{0, 1 << 0};
    Value<float> gravityScore{0, 1 << 1};
    Value<float> concentrationScore{0, 1 << 2};
    Value<float> accumulatedFatigue{0, 1 << 3};
    Value<float> individualPeakFrequency{0, 1 << 4};
};

class Client
{
public:
    Client() = default;

    Client(const std::string& ip, int port);

    ~Client();

    void Close();

    void Init(const std::string& ip, int port);

    void SendData(const Data& data, uint32_t field_flags);

private:
    int client_;
    sockaddr_in servAddr_;
};

} // namespace socket_communication
