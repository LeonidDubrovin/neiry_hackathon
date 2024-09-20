#include <chrono>
#include <format>
#include <fstream>
#include <future>
#include <iostream>
#include <thread>

#include "ExampleUtils.hpp"

#include "Capsule/CClient.h"
#include "Capsule/CDevice.h"

using namespace std::chrono_literals;

// objects for establishing a connection with the device and capsule
clCClient client = nullptr;
clCDeviceLocator locator = nullptr;
clCDevice device = nullptr;

uint32_t s_time = 0;
uint32_t deviceConnectionTime = 0;
bool writeCsv = false;

bool clientStopRequested = false;
bool clientDisconnecting = false;

std::ofstream ppgStream;
void onPPGData(clCDevice, clCPPGTimedData ppgData) {
    const int32_t count = clCPPGTimedData_GetCount(ppgData);
    std::cout << "PPG raw data received " << count << " samples" << std::endl;

    if (!writeCsv || !ppgStream.is_open()) {
        return;
    }
    for (int32_t i = 0; i < count; ++i) {
        ppgStream << clCPPGTimedData_GetTimepoint(ppgData, i) << ',' << clCPPGTimedData_GetValue(ppgData, i) << std::endl;
    }
}

std::ofstream memsStream;
void onMEMSData(clCDevice, clCMEMSTimedData memsData) {
    const int32_t count = clCMEMSTimedData_GetCount(memsData);
    std::cout << "MEMS raw data received " << count << " samples" << std::endl;

    if (!writeCsv || !memsStream.is_open()) {
        return;
    }
    for (int32_t i = 0; i < count; ++i) {
        const auto acc = clCMEMSTimedData_GetAccelerometer(memsData, i);
        const auto gyro = clCMEMSTimedData_GetGyroscope(memsData, i);
        memsStream << clCMEMSTimedData_GetTimepoint(memsData, i) << ','
                   << acc.x << ',' << acc.y << ',' << acc.z << ','
                   << gyro.x << ',' << gyro.y << ',' << gyro.z << std::endl;
    }
}

std::ofstream eegStream;
void onEEGData(clCDevice, clCEEGTimedData eegData) {
    const int32_t samples = clCEEGTimedData_GetSamplesCount(eegData);
    const int32_t channels = clCEEGTimedData_GetChannelsCount(eegData);
    std::cout << "EEG raw data received " << channels << " channels and " << samples << " samples" << std::endl;

    if (!writeCsv || !eegStream.is_open()) {
        return;
    }
    for (int32_t i = 0; i < samples; ++i) {
        eegStream << clCEEGTimedData_GetTimepoint(eegData, i) << ',';
        for (int32_t j = 0; j < channels; ++j) {
            eegStream << clCEEGTimedData_GetValue(eegData, j, i);
            if (j == channels - 1) {
                eegStream << std::endl;
            } else {
                eegStream << ',';
            }
        }
    }
}

void onConnectionStateChanged([[maybe_unused]] clCDevice device, clCDeviceConnectionState state) {
    // status of the device changed
    if (state != clC_SE_Connected) {
        std::cout << "Device disconnected" << std::endl;
        return;
    }
    std::cout << "Device connected" << std::endl;
    deviceConnectionTime = s_time;

    if (writeCsv) {
        ppgStream.open("device_ppg.csv");
        ppgStream << "timestamp,value\n";
        memsStream.open("device_mems.csv");
        memsStream << "timestamp,ax,ay,az,gx,gy,gz\n";
        eegStream.open("device_eeg.csv");
        eegStream << "timestamp,";
    }

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
        eegStream << clCString_CStr(channelName);
        if (i == channelsCount - 1) {
            eegStream << std::endl;
        } else {
            eegStream << ',';
        }
        clCString_Free(channelName);
    }
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
    clCDeviceDelegatePPGData onPPGDataEvent = clCDevice_GetOnPPGDataEvent(device);
    clCDeviceDelegateMEMSData onMEMSDataEvent = clCDevice_GetOnMEMSDataEvent(device);
    clCDeviceDelegateEEGData onEEGDataEvent = clCDevice_GetOnEEGDataEvent(device);
    //  Initialize device events
    clCDeviceDelegateConnectionState_Set(onConnectionStateChangedEvent, onConnectionStateChanged);
    clCDeviceDelegatePPGData_Set(onPPGDataEvent, onPPGData);
    clCDeviceDelegateMEMSData_Set(onMEMSDataEvent, onMEMSData);
    clCDeviceDelegateEEGData_Set(onEEGDataEvent, onEEGData);
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
        if (deviceConnectionTime > 0 && s_time == deviceConnectionTime + 2 * kMsSec) {
            clCDevice_SwitchMode(device, clC_DM_Signal);
            clCDevice_SwitchMode(device, clC_DM_StartPPG);
            clCDevice_SwitchMode(device, clC_DM_StartMEMS);
        }
        if (device == nullptr && s_time == 100 * kMsSec) {
            clientStopRequested = true;
        }
        if (clientStopRequested && !clientDisconnecting) {
            // Destroy all objects that were created at the end of the program and
            // break the connection in callback onDisconnected
            clCClient_Disconnect(client);
            clientDisconnecting = true;
        }
    }

    if (ppgStream.is_open()) {
        ppgStream.close();
    }
    if (memsStream.is_open()) {
        memsStream.close();
    }
    if (eegStream.is_open()) {
        eegStream.close();
    }

    exit(0);
}

int main(int argc, char* argv[]) {
    parseArgs(argc, argv, nullptr, nullptr, &writeCsv);

    std::cout << std::boolalpha << "Write to CSV: " << writeCsv << std::endl;
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
