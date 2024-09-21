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
    Value<float> fatigueScore{0, 0x01};
    Value<float> gravityScore{0, 0x02};
    Value<float> concentrationScore{0, 0x04};
    Value<float> accumulatedFatigue{0, 0x08};
    Value<float> individualPeakFrequency{0, 0x16};
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
