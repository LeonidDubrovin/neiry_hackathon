#include <chrono>
#include <format>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>

#include "ExampleUtils.hpp"

#include "Capsule/CClient.h"
#include "Capsule/CDevice.h"
#include "Capsule/CLicenseManager.h"
#include "Capsule/CSession.h"

using namespace std::chrono_literals;

// objects for establishing a connection with the device and capsule
clCClient client = nullptr;
clCSession session = nullptr;
clCDeviceLocator locator = nullptr;
clCDevice device = nullptr;

uint32_t s_time = 0;
uint32_t deviceConnectionTime = 0;

bool writeCsv = false;
bool bipolarMode = false;

bool clientStopRequested = false;
bool clientDisconnecting = false;

// License key
std::string licenseKey;

void onLicenseVerified(clCLicenseManager, bool result, clCLicenseError) {
    std::cout << "License verification result: " << std::boolalpha << result << std::endl;
    if (!result) {
        std::cerr << "License verification failed. Exiting..." << std::endl;
        clientStopRequested = true;
    }
}

std::ofstream sessionEegStream;
void onSessionEEGData(clCSession, clCEEGTimedData eegData) {
    const int32_t samples = clCEEGTimedData_GetSamplesCount(eegData);
    const int32_t channels = clCEEGTimedData_GetChannelsCount(eegData);
    std::cout << "Session EEG data received " << channels << " channels and " << samples << " samples" << std::endl;

    if (!writeCsv || !sessionEegStream.is_open()) {
        return;
    }
    for (int32_t i = 0; i < samples; ++i) {
        sessionEegStream << clCEEGTimedData_GetTimepoint(eegData, i) << ',';
        for (int32_t j = 0; j < channels; ++j) {
            sessionEegStream << clCEEGTimedData_GetValue(eegData, j, i);
            if (j == channels - 1) {
                sessionEegStream << std::endl;
            } else {
                sessionEegStream << ',';
            }
        }
    }
}

void onSessionStarted(clCSession session) {
    std::cout << "Session started" << std::endl;
    clCString sessionUUID = clCSession_GetSessionUUID(session);
    std::cout << "Session UUID: " << clCString_CStr(sessionUUID) << std::endl;
    clCString_Free(sessionUUID);
    clCSession_MarkActivity(session, clCUserActivity1);

    if (writeCsv) {
        sessionEegStream.open("session_eeg.csv");
        sessionEegStream << "timestamp,";
        clCDeviceChannelNames channelNames = clCDevice_GetChannelNames(device);
        auto channelsCount = clCDevice_GetChannelsCount(channelNames);
        if (clCSession_IsBipolarMode(session)) {
            channelsCount /= 2;
        }
        for (int32_t i = 0; i < channelsCount; ++i) {
            sessionEegStream << "channel_" << i;
            if (i == channelsCount - 1) {
                sessionEegStream << std::endl;
            } else {
                sessionEegStream << ',';
            }
        }
    }

    clCDevice_SwitchMode(device, clC_DM_Signal);
    clCDevice_SwitchMode(device, clC_DM_StartPPG);
    clCDevice_SwitchMode(device, clC_DM_StartMEMS);
}

void onSessionError([[maybe_unused]] clCSession session, clCSessionError error) {
    std::cerr << "Session error: " << static_cast<int>(error) << std::endl;
}

void onSessionStopped([[maybe_unused]] clCSession session) {
    std::cout << "Session stopped" << std::endl;
}

void onConnectionStateChanged([[maybe_unused]] clCDevice device, clCDeviceConnectionState state) {
    // status of the device changed
    if (state != clC_SE_Connected) {
        std::cout << "Device disconnected" << std::endl;
        return;
    }
    std::cout << "Device connected" << std::endl;
    deviceConnectionTime = s_time;

    // get channel names
    clCDeviceChannelNames channelNames = clCDevice_GetChannelNames(device);
    const auto channelsCount = clCDevice_GetChannelsCount(channelNames);
    std::cout << "Device has " << channelsCount << " channels: [";
    for (int32_t i = 0; i < channelsCount; ++i) {
        clCString channelName = clCDevice_GetChannelNameByIndex(channelNames, i);
        std::cout << clCString_CStr(channelName) << ", ";
        clCString_Free(channelName);
    }
    std::cout << "\b\b]" << std::endl;
    for (int32_t i = 0; i < channelsCount; ++i) {
        clCString channelName = clCDevice_GetChannelNameByIndex(channelNames, i);
        std::cout << "\tChannel " << clCString_CStr(channelName)
                  << " has index " << clCDevice_GetChannelIndexByName(channelNames, clCString_CStr(channelName)) << std::endl;
        clCString_Free(channelName);
    }

    auto licenseManager = clCClient_GetLicenseManager(client);
    auto onLicenseVerifiedEvent = clCLicenseManager_GetOnLicenseVerifiedEvent(licenseManager);
    clCLicenseManagerDelegateLicenseVerified_Set(onLicenseVerifiedEvent, onLicenseVerified);
    clCLicenseManager_VerifyLicense(licenseManager, licenseKey.c_str(), device);
}

void onDeviceList(clCDeviceLocator locator, clCDeviceInfoList devices, clCDeviceLocatorFailReason error) {
    // device connected
    if (device != nullptr) {
        return;
    }

    if (error != clC_DeviceLocatorFailReason_OK) {
        switch (error) {
        case clC_DeviceLocatorFailReason_BluetoothDisabled:
            std::cerr << "Bluetooth adapter not found or disabled";
            break;
        default:
            std::cerr << "Unknown error occurred";
        }
        std::cerr << ". Exiting..." << std::endl;
        clientStopRequested = true;
        return;
    }

    if (clCDeviceInfoList_GetCount(devices) == 0) {
        std::cerr << "Empty device list. Exiting..." << std::endl;
        clientStopRequested = true;
        return;
    }

    // print information about all found devices
    const int32_t count = clCDeviceInfoList_GetCount(devices);
    std::cout << "Devices: " << count << std::endl;
    for (int i = 0; i < count; i++) {
        const clCDeviceInfo deviceDescriptor = clCDeviceInfoList_GetDeviceInfo(devices, i);
        clCString deviceDescription = clCDeviceInfo_GetDescription(deviceDescriptor);
        std::cout << "\t " << clCString_CStr(deviceDescription) << std::endl;
        clCString_Free(deviceDescription);
    }

    // select device and connect
    const clCDeviceInfo deviceDescriptor = clCDeviceInfoList_GetDeviceInfo(devices, 0);
    clCString deviceID = clCDeviceInfo_GetID(deviceDescriptor);
    device = clCDeviceLocator_CreateDevice(locator, clCString_CStr(deviceID));
    clCString_Free(deviceID);
    if (device == nullptr) {
        std::cerr << "Failed to create device. Exiting..." << std::endl;
        clientStopRequested = true;
        return;
    }
    // Get device events
    clCDeviceDelegateDeviceConnectionState onConnectionStateChangedEvent = clCDevice_GetOnConnectionStateChangedEvent(device);
    //  Initialize device events
    clCDeviceDelegateConnectionState_Set(onConnectionStateChangedEvent, onConnectionStateChanged);
    //  connect to the device
    clCDevice_Connect(device);
}

void onConnected(clCClient client) {
    // As soon as the capsule has confirmed the connection,
    // launch the locator to search for the selected device.
    // when receiving a list of available devices, control is transferred to onDeviceList
    std::cout << "Connected" << std::endl;
    locator = clCClient_ChooseDeviceType(client, clC_DT_NeiryBand);
    clCDeviceLocatorDelegateDeviceInfoList onDevicesEvent = clCDeviceLocator_GetOnDevicesEvent(locator);
    clCDeviceLocatorDelegateDeviceInfoList_Set(onDevicesEvent, onDeviceList);
    clCDeviceLocator_RequestDevices(locator, 15);
}

void onError([[maybe_unused]] clCClient client, clCError error) {
    std::cerr << "Error: " << static_cast<int>(error) << std::endl;
}

void onDisconnected(clCClient client, clCDisconnectReason reason) {
    // destroy all objects
    std::cout << "Disconnected: " << static_cast<int>(reason) << std::endl;

    if (session) {
        clCSession_Destroy(session);
        session = nullptr;
    }
    if (device) {
        clCDevice_Release(device);
        device = nullptr;
    }
    clCDeviceLocator_Destroy(locator);
    locator = nullptr;
    clCClient_Destroy(client);
    ::client = nullptr;
    std::cout << "End work" << std::endl;
}

void ClientLoop() {
    static constexpr uint32_t kMsSec = 1000U;
    s_time = 0;
    while (client) {
        // Update does all the work and must be called regularly to process events.
        clCClient_Update(client);
        std::this_thread::sleep_for(50ms);
        s_time += 50;
        // Create a session object. The session is necessary for
        // the client to work with the device - obtaining user state data
        // or information about the device
        if (deviceConnectionTime && s_time == deviceConnectionTime + 2 * kMsSec && !session) {
            // Create session
            auto error = clC_Error_OK;
            if (bipolarMode) {
                session = clCClient_CreateSessionWithError(client, device, &error);
            } else {
                session = clCClient_CreateSessionWithMonopolarChannelsWithError(client, device, &error);
            }

            // Get session events
            clCSessionDelegate onSessionStartedEvent = clCSession_GetOnSessionStartedEvent(session);
            clCSessionDelegateSessionError onSessionErrorEvent = clCSession_GetOnErrorEvent(session);
            clCSessionDelegate onSessionStoppedEvent = clCSession_GetOnSessionStoppedEvent(session);
            clCSessionDelegateSessionEEGData onSessionEEGDataEvent = clCSession_GetOnSessionEEGDataEvent(session);

            // Initialize session events
            clCSessionDelegate_Set(onSessionStartedEvent, onSessionStarted);
            clCSessionDelegateSessionError_Set(onSessionErrorEvent, onSessionError);
            clCSessionDelegate_Set(onSessionStoppedEvent, onSessionStopped);
            clCSessionDelegateSessionEEGData_Set(onSessionEEGDataEvent, onSessionEEGData);

            // Start session
            const clCSessionState state = clCSession_GetSessionState(session);
            clCSession_Start(session);
            std::cout << "Session state: " << static_cast<int>(state) << std::endl;
        }
        if (!session && s_time == 100 * kMsSec) {
            // Destroy all objects that were created at the end of the program and
            // break the connection in callback onDisconnected
            clientStopRequested = true;
        }
        if (clientStopRequested && !clientDisconnecting) {
            // Destroy all objects that were created at the end of the program and
            // break the connection in callback onDisconnected
            clCClient_Disconnect(client);
            clientDisconnecting = true;
        }
    }

    if (sessionEegStream.is_open()) {
        sessionEegStream.close();
    }

    exit(0);
}

int main(int argc, char* argv[]) {
    parseArgs(argc, argv, &licenseKey, &bipolarMode, &writeCsv);

    std::cout << std::boolalpha
              << "Bipolar mode: " << bipolarMode << '\n'
              << "Write to CSV: " << writeCsv << std::endl;

    std::cout << "To quit the example type 'q' and press enter" << std::endl;
    // Getting the version of the library
    // and an example of working with a clCString
    {
        clCString strPtr = clCClient_GetVersionString();
        std::cout << "Version of the library: " << clCString_CStr(strPtr) << std::endl;
        clCString_Free(strPtr);
    }
    // Create client
    client = clCClient_CreateWithName("CapsuleClientExample");

    // Get client events
    clCClientDelegate onConnectedEvent = clCClient_GetOnConnectedEvent(client);
    clCClientDelegateError onErrorEvent = clCClient_GetOnErrorEvent(client);
    clCClientDelegateDisconnectReason onDisconnectedEvent = clCClient_GetOnDisconnectedEvent(client);

    // Initialize client events
    clCClientDelegate_Set(onConnectedEvent, onConnected);
    clCClientDelegateError_Set(onErrorEvent, onError);
    clCClientDelegateDisconnectReason_Set(onDisconnectedEvent, onDisconnected);

    clCClient_Connect(client, "inproc://capsule");

    auto future = std::async(std::launch::async, ClientLoop);
    char input;
    while (std::cin >> input) {
        if (input == 'q' || input == 'Q') {
            clientStopRequested = true;
            break;
        }
    }
    future.wait();

    return 0;
}
