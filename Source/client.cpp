#include <client.hpp>
#include <WS2tcpip.h>

namespace socket_communication
{
namespace
{
bool TestWinsock() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("WSAStartup failed: %d\n", iResult);
        return false;
    }
    return true;
}

void SerializeData(const Data &data, char *buffer, size_t &buffer_size, uint32_t field_flags) {
    size_t buff_curr_size = 0;

    memcpy(buffer, &field_flags, sizeof(uint32_t));
    buff_curr_size += sizeof(uint32_t);

    memcpy(buffer + buff_curr_size, &data.fatigueScore.value, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.gravityScore.value, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.concentrationScore.value, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.accumulatedFatigue.value, sizeof(float));
    buff_curr_size += sizeof(float);

    memcpy(buffer + buff_curr_size, &data.individualPeakFrequency.value, sizeof(float));
    buff_curr_size += sizeof(float);

    buffer_size = buff_curr_size;
}

bool ConnectToServer(int &sock, sockaddr_in &server_address) {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Setting socket failed\n";
        WSACleanup();
        return false;
    }

    if (connect(sock, (sockaddr *) &server_address, sizeof(server_address)) < 0) {
        closesocket(sock);
        return false;
    }

    return true;
}

void DisconnectFromServer(int &sock)
{
    std::cout << "[Client]: disconnect from server" << std::endl;
    closesocket(sock);
    WSACleanup();
}

} // namespace

Client::Client(const std::string &ip, int port)
        : client_{}
        , servAddr_{}
{
    Init(ip, port);
}

Client::~Client() {
    Close();
}

void Client::Close() {
    int status = closesocket(client_);
    if (status != 0) {
        std::cout << "[Client]: ERROR closing socket" << std::endl;
    }
    WSACleanup();
}

void Client::Init(const std::string &ip, int port) {
    if (!TestWinsock()) {
        std::cout << "[Client]: ERROR test socket" << std::endl;
        exit(1);
    }

    servAddr_.sin_family = AF_INET;
    servAddr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &servAddr_.sin_addr);
    while (!ConnectToServer(client_, servAddr_)) {
        std::cerr << "Connect to server failed, try again after 5 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

void Client::SendData(const Data &data, uint32_t field_flags) {
    char buffer[1024];
    size_t buffer_size = 0;
    SerializeData(data, buffer, buffer_size, field_flags);

    std::cout << "[Client]: Sending data" << std::endl;
    size_t bytes_sent = send(client_, buffer, static_cast<int>(buffer_size), 0);
    if (bytes_sent == SOCKET_ERROR) {
        std::cout << "[Client]: Send failed: " << WSAGetLastError() << std::endl;
        DisconnectFromServer(client_);
        while (!ConnectToServer(client_, servAddr_)) {
            std::cerr << "Connect to server failed, try again...\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    } else {
        std::cout << "[Client]: Bytes Sent " << bytes_sent << std::endl;
    }

//        char response[1024];
//        std::cout << "[Client]: Receiving data" << std::endl;
//        size_t bytes_received = recv(client_, response, sizeof(response), 0);
//        if (bytes_received <= 0) {
//            std::cerr << "Connection lost...\n";
//            DisconnectFromServer(client_);
//            while (!ConnectToServer(client_, servAddr_)) {
//                std::cerr << "Connect to server failed, try again...\n";
//                std::this_thread::sleep_for(std::chrono::seconds(5));
//            }
//            std::cout << "Connection OK\n";
//        } else {
//            std::cout << "Receive from server: " << response << std::endl;
//        }
}

}  // namespace socket_communication