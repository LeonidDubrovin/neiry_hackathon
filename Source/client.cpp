#include <client.hpp>
#include <WS2tcpip.h>
//#include <unistd.h>

namespace socket_communication {
namespace {
bool TestWinsock()
{
    // Initialize Winsock
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }
    return true;
}

void SerializeData(const Data& data, char* buffer, size_t& buffer_size) {
    size_t buff_curr_size = 0;

    memcpy(buffer + buff_curr_size, &data.fatigueScore, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.gravityScore, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.concentrationScore, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.accumulatedFatigue, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.individualPeakFrequency, sizeof(float));
    buff_curr_size += sizeof(float);

    buffer_size = buff_curr_size;
}
} // namespace

Client::Client()
{}

Client::Client(const std::string& ip, int port)
{
    Init(ip, port);
}

Client::~Client()
{
    Close();
}

void Client::Close()
{
    int status = closesocket(client_);
    if (status != 0)
    {
        std::cout << "[Client]: ERROR closing socket" << std::endl;
    }
    WSACleanup();
}

void Client::Init(const std::string& ip, int port)
{
    if (!TestWinsock())
    {
        std::cout << "[Client]: ERROR test socket" << std::endl;
        exit(1);
    }

    client_ = socket(AF_INET, SOCK_STREAM, 0);
    if (client_ == INVALID_SOCKET) {
        std::cout << "[Client]: ERROR establishing socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        exit(1);
    }

    bool connected = false;
    int connection_attempts = 5;

    while (!connected) {
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr);

        if (connect(client_, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) == 0)
        {
            connected = true;
            std::cout << "[Client]: Cpp socket client connected." << std::endl;
        } else
        {
            connection_attempts++;
            std::cout << "[Client]: Error connecting to port " << port << ", attempts: " << connection_attempts << std::endl;
        }
    }
}

void Client::SendData(const Data& data)
{
    char buffer[1024];
    size_t buffer_size = 0;
    SerializeData(data, buffer, buffer_size);

    int result = send(client_, buffer, buffer_size, 0);
    if (result == SOCKET_ERROR) {
        std::cout << "[Client]: Send failed: " << WSAGetLastError() << std::endl;
    } else
    {
        std::cout << "[Client]: Bytes Sent " << result << std::endl;
    }
}

}  // namespace socket_communication