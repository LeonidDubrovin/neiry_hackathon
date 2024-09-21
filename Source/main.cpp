#include <chrono>
#include <format>
#include <future>
#include <iostream>
#include <thread>

#include "ExampleUtils.hpp"

#include "CClientAPI.h"
#include <client.hpp>

using namespace std::chrono_literals;

// objects for establishing a connection with the device and capsule
clCClient client = nullptr;
clCSession session = nullptr;
clCNFBCalibrator calibrator = nullptr;
clCDeviceLocator locator = nullptr;
clCDevice device = nullptr;

// object for processing raw EEG signal and receiving NFB rhythms
clCNFB nfb = nullptr;
// object for processing the user's EEG rhythms and obtaining productivity score and other data
clCNFBMetricProductivity productivity = nullptr;
// object for processing the user's ECG rhythms
clCCardio cardio = nullptr;
// object for processing the user's MEMS
clCMEMS mems = nullptr;

uint32_t s_time = 0;
uint32_t deviceConnectionTime = 0;

bool clientStopRequested = false;
bool clientDisconnecting = false;

// License key
std::string licenseKey;


std::shared_ptr<socket_communication::Client> socketClient;


void onResistances([[maybe_unused]] clCDevice device, clCResistances resistances) {
    // Get total number of resistance channels.
    const int32_t count = clCResistances_GetCount(resistances);
    std::cout << "Resistances: " << count << std::endl;
    for (int32_t i = 0; i < count; ++i) {
        const char* channelName = clCString_CStr(clCResistances_GetChannelName(resistances, i));
        const float value = clCResistances_GetValue(resistances, i);
        std::cout << "\t " << channelName << " = " << value << std::endl;
    }
}

void onNFBInitializedEvent(clCNFB nfb) {
    // Subscribe to the data that we are interested in
    const clCNFBCallResult resultAlpha = clCNFB_AddFeedbackFunction(nfb, "alpha");
    const clCNFBCallResult resultBeta = clCNFB_AddFeedbackFunction(nfb, "beta");
    const clCNFBCallResult resultTheta = clCNFB_AddFeedbackFunction(nfb, "theta");

    if (resultAlpha == clC_NFB_Success && resultBeta == clC_NFB_Success && resultTheta == clC_NFB_Success) {
        std::cout << "Call addFeedback succeeded" << std::endl;
    }
}

void onUpdateUserState([[maybe_unused]] clCNFB nfb, const clCNFBUserState* userState) {
    // Getting NFB user data
    // if artifacts or weak resistance on the electrodes are observed,
    // the data will not be changed
    std::cout << "NFB update state: alpha = " << userState->feedbackData[0] << " , beta = " << userState->feedbackData[1] << " , theta = " << userState->feedbackData[2] << std::endl;
}

void onNFBErrorEvent([[maybe_unused]] clCNFB nfb, const char* error) {
    std::cerr << "NFB error: " << error << std::endl;
}

void onProductivityArtifacts([[maybe_unused]] clCNFBMetricProductivity productivity, [[maybe_unused]] const clCNFBUserArtifacts*) {
    std::cout << "Productivity received information about artifacts" << std::endl;
}

void onProductivityIndividualInfo([[maybe_unused]] clCNFBMetricProductivity productivity,
                                  [[maybe_unused]] const clCNFBMetricsProductivityIndividualIndexes*) {
    std::cout << "Productivity received individual indexes" << std::endl;
}

void onProductivtyScoreUpdate([[maybe_unused]] clCNFBMetricProductivity productivity, float productivityScore) {
    std::cout << "Productivity received productivity score: " << productivityScore << std::endl;
}

void onProductivityValuesUpdate(clCNFBMetricProductivity, const clCNFBMetricsProductivityValues* values) {
    std::cout << "Productivity values update:\n"
              << "\tFatigue Score: " << values->fatigueScore << '\n'
              << "\tGravity Score: " << values->gravityScore << '\n'
              << "\tConcentration Score: " << values->concentrationScore << '\n'
              << "\tRelaxation Score: " << values->relaxationScore << '\n'
              << "\tAccumulated Fatigue: " << values->accumulatedFatigue << '\n'
              << "\tFatigue Growth Rate: " << values->fatigueGrowthRate << std::endl;

    socket_communication::Data data{};
    data.fatigueScore.value = values->fatigueScore;
    data.gravityScore.value = values->gravityScore;
    data.concentrationScore.value = values->concentrationScore;
    data.accumulatedFatigue.value = values->accumulatedFatigue;

    uint32_t field_flags = data.fatigueScore.code | data.gravityScore.code | data.concentrationScore.code | data.accumulatedFatigue.code;
    socketClient->SendData(data, field_flags);
}

void onCardioIndexesUpdate([[maybe_unused]] clCCardio cardio, clCCardioData data) {
    std::cout << "Cardio indexes update: (artifacted " << data.artifacted
              << "), Kaplan's index " << data.kaplanIndex << ", HR " << data.heartRate
              << ", stress index " << data.stressIndex << std::endl;
}

void onMEMSUpdate([[maybe_unused]] clCMEMS mems, clCMEMSTimedData data) {
    const int32_t count = clCMEMSTimedData_GetCount(data);
    std::cout << "MEMS update: showing 1 of " << count << " values" << std::endl;
    const clCPoint3d accelerometer = clCMEMSTimedData_GetAccelerometer(data, 0);
    const clCPoint3d gyroscope = clCMEMSTimedData_GetGyroscope(data, 0);
    const auto timepoint = clCMEMSTimedData_GetTimepoint(data, 0);
    std::cout << "\taccelerometer: " << accelerometer.x << ", "
              << accelerometer.y << ", " << accelerometer.z << std::endl;
    std::cout << "\tgyroscope: " << gyroscope.x << ", "
              << gyroscope.y << ", " << gyroscope.z << std::endl;
    std::cout << "\ttime: " << std::to_string(timepoint) << std::endl;
}

void onCalibrated(clCNFBCalibrator, const clCIndividualNFBData* data, clCIndividualNFBCalibrationFailReason failReason) {
    if (data == nullptr || failReason != clC_IndividualNFBCalibrationFailReason_None) {
        std::cerr << "Calibration failed";
        switch (failReason) {
        case clC_IndividualNFBCalibrationFailReason_TooManyArtifacts:
            std::cerr << ": Too many artifacts";
            break;
        case clC_IndividualNFBCalibrationFailReason_PeakIsABorder:
            std::cerr << ": Alpha peak matches one of the alpha tange borders";
            break;
        default:
            std::cerr << ": Reason unknown";
        }
        std::cerr << ". Exiting..." << std::endl;
        clientStopRequested = true;
        return;
    }
//    std::cout << "Calibration suceeded. IAF:" << data->individualFrequency << std::endl;

    socket_communication::Data dataForSend{};
    dataForSend.individualPeakFrequency.value = data->individualPeakFrequency;
    uint32_t field_flags = dataForSend.individualPeakFrequency.code;
    socketClient->SendData(dataForSend, field_flags);
}

void onCalibratorReady(clCNFBCalibrator calibrator) {
    std::cout << "Calibrator is ready to calibrate NFB\n"
              << "Close your eyes for 30 seconds" << std::endl;
    clCError error = clC_Error_OK;
    clCNFBCalibrator_CalibrateIndividualNFBQuick(calibrator, &error);
}

void onLicenseVerified(clCLicenseManager, bool result, clCLicenseError) {
    std::cout << "License verification result: " << std::boolalpha << result << std::endl;
    if (!result) {
        std::cerr << "License verification failed. Exiting..." << std::endl;
        clientStopRequested = true;
    }
}

void onSessionStarted(clCSession session) {
    std::cout << "Session started" << std::endl;
    const char* sessionUUID = clCString_CStr(clCSession_GetSessionUUID(session));
    std::cout << "Session UUID: " << sessionUUID << std::endl;
    clCSession_MarkActivity(session, clCUserActivity1);

    calibrator = clCNFBCalibrator_CreateOrGet(session);
    clCNFBCalibratorDelegateIndividualNFBCalibrated onCalibratedEvent = clCNFBCalibrator_GetOnIndividualNFBCalibratedEvent(calibrator);
    clCNFBCalibratorDelegateIndividualNFBCalibrated_Set(onCalibratedEvent, onCalibrated);
    clCNFBCalibratorDelegateReadyToCalibrate onCalibratorReadyEvent = clCNFBCalibrator_GetOnReadyToCalibrate(calibrator);
    clCNFBCalibratorDelegateReadyToCalibrate_Set(onCalibratorReadyEvent, onCalibratorReady);

    // Create NFB - NeiryFeedBack
    nfb = clCNFB_Create(session);
    // get NFB events
    clCNFBDelegate delegateInit = clCNFB_GetOnInitializedEvent(nfb);
    clCNFBDelegateNFBUserState delegateState = clCNFB_GetOnUserStateChangedEvent(nfb);
    clCNFBDelegateString delegateError = clCNFB_GetOnErrorEvent(nfb);
    // Initialize NFB events
    clCNFBDelegate_Set(delegateInit, onNFBInitializedEvent);
    clCNFBDelegateNFBUserState_Set(delegateState, onUpdateUserState);
    clCNFBDelegateString_Set(delegateError, onNFBErrorEvent);
    // Initialize NFB
    clCNFB_Initialize(nfb);

    // Productivity metrics is an algorithm that uses custom rhythms
    // and an IAPF and other indicators to calculate the productivity score
    // Create Productivity metric - math constants are better not to change yet
    clCError error = clC_Error_OK;
    productivity = clCNFBMetricsProductivity_Create(session, "logs", 0.05, 0.05, 0.001, &error);
    if (error != clC_Error_OK) {
        std::cerr << "Failed to create Productivity classifier" << std::endl;
        return;
    }
    // get Productivity events
    clCNFBMetricsProductivityIndividualDelegate delegateMeasured = clCNFBMetricsProductivity_GetOnIndividualMeasuredEvent(productivity);
    clCNFBMetricsProductivityArtifactsDelegate delegateArtifacts = clCNFBMetricsProductivity_GetOnArtifactsEvent(productivity);
    clCNFBMetricsProductivityEventDelegate delegateUserUpdate = clCNFBMetricsProductivity_GetOnUpdateEvent_1min(productivity);
    clCNFBMetricsProductivityValuesDelegate delegateValuesUpdate = clCNFBMetricsProductivity_GetOnProductivityValuesEvent(productivity);
    // Initialize Productivity events
    clCNFBMetricsProductivity_IndividualMeasuredEvent_Set(delegateMeasured, onProductivityIndividualInfo);
    clCNFBMetricsProductivity_ArtifactsEvent_Set(delegateArtifacts, onProductivityArtifacts);
    clCNFBMetricsProductivity_UpdateEvent_Set(delegateUserUpdate, onProductivtyScoreUpdate);
    clCNFBMetricsProductivity_ValuesEvent_Set(delegateValuesUpdate, onProductivityValuesUpdate);
    // Initialize Productivity
    clCNFBMetricsProductivity_InitializeNFB(productivity, "", &error);

    clCDevice_SwitchMode(device, clC_DM_Signal);
    cardio = clCCardio_Create(session);
    if (cardio == nullptr) {
        std::cerr << "Failed to create cardio classifier" << std::endl;
        return;
    }
    clCCardioIndexesDelegate delegateCardio = clCCardio_GetOnIndexesUpdateEvent(cardio);
    clCCardioDelegateIndexesUpdate_Set(delegateCardio, onCardioIndexesUpdate);
    clCCardio_Initialize(cardio);
    clCDevice_SwitchMode(device, clC_DM_StartPPG);

    mems = clCMEMS_Create(session);
    if (mems == nullptr) {
        std::cerr << "Failed to create MEMS classifier" << std::endl;
        return;
    }
    clCMEMSTimedDataDelegate delegateMEMS = clCMEMS_GetOnMEMSTimedDataUpdateEvent(mems);
    clCMEMSDelegateMEMSTimedDataUpdate_Set(delegateMEMS, onMEMSUpdate);
    clCMEMS_Initialize(mems);

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
        std::cout << clCString_CStr(clCDevice_GetChannelNameByIndex(channelNames, i)) << ", ";
    }
    std::cout << "\b\b]" << std::endl;
    for (int32_t i = 0; i < channelsCount; ++i) {
        const char* channel = clCString_CStr(clCDevice_GetChannelNameByIndex(channelNames, i));
        std::cout << "\tChannel " << channel << " has index " << clCDevice_GetChannelIndexByName(channelNames, channel) << std::endl;
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
        clCDeviceInfo deviceDescriptor = clCDeviceInfoList_GetDeviceInfo(devices, i);
        const char* deviceDescription = clCString_CStr(clCDeviceInfo_GetDescription(deviceDescriptor));
        std::cout << "\t " << deviceDescription << std::endl;
    }

    // select device and connect
    clCDeviceInfo deviceDescriptor = clCDeviceInfoList_GetDeviceInfo(devices, 0);
    const char* deviceID = clCString_CStr(clCDeviceInfo_GetID(deviceDescriptor));
    device = clCDeviceLocator_CreateDevice(locator, deviceID);
    if (device == nullptr) {
        std::cerr << "Failed to create device. Exiting..." << std::endl;
        clientStopRequested = true;
        return;
    }
    // Get device events
    clCDeviceDelegateResistances onResistancesEvent = clCDevice_GetOnResistancesEvent(device);
    clCDeviceDelegateDeviceConnectionState onConnectionStateChangedEvent = clCDevice_GetOnConnectionStateChangedEvent(device);
    //  Initialize device events
    clCDeviceDelegateResistances_Set(onResistancesEvent, onResistances);
    clCDeviceDelegateConnectionState_Set(onConnectionStateChangedEvent, onConnectionStateChanged);
    //  Сonnect to the device
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

    if (mems) {
        clCMEMS_Destroy(mems);
        mems = nullptr;
    }
    if (cardio) {
        clCCardio_Destroy(cardio);
        cardio = nullptr;
    }
    if (nfb) {
        clCNFB_Destroy(nfb);
        nfb = nullptr;
    }
    if (productivity) {
        clCNFBMetricsProductivity_Destroy(productivity);
        productivity = nullptr;
    }
    calibrator = nullptr;
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
            session = clCClient_CreateSessionWithError(client, device, &error);

            // Get session events
            clCSessionDelegate onSessionStartedEvent = clCSession_GetOnSessionStartedEvent(session);
            clCSessionDelegateSessionError onSessionErrorEvent = clCSession_GetOnErrorEvent(session);
            clCSessionDelegate onSessionStoppedEvent = clCSession_GetOnSessionStoppedEvent(session);

            // Initialize session events
            clCSessionDelegate_Set(onSessionStartedEvent, onSessionStarted);
            clCSessionDelegateSessionError_Set(onSessionErrorEvent, onSessionError);
            clCSessionDelegate_Set(onSessionStoppedEvent, onSessionStopped);

            // Start session
            const clCSessionState state = clCSession_GetSessionState(session);
            clCSession_Start(session);
            std::cout << "Session state: " << static_cast<int>(state) << std::endl;
        }
        if (!session && s_time == 100 * kMsSec) {
            clientStopRequested = true;
        }
        if (clientStopRequested && !clientDisconnecting) {
            // Destroy all objects that were created at the end of the program and
            // break the connection in callback onDisconnected
            clCClient_Disconnect(client);
            clientDisconnecting = true;
        }
        if (static bool printFirmware = true; printFirmware && device && clCDevice_FirmwareVersionReceived(device)) {
            clCError error = clCError::clC_Error_OK;
            const auto firmware = clCDevice_GetFirmwareVersion(device, &error);
            std::cout << "Device firmware: " << clCString_CStr(firmware) << std::endl;
            clCString_Free(firmware);
            printFirmware = false;
        }
    }

    exit(0);
}

int main(int argc, char* argv[]) {
    parseArgs(argc, argv, &licenseKey, nullptr, nullptr);

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
    // Getting the client name of the library
    // and an example of working with a clCString
    {
        clCString strPtr = clCClient_GetClientName(client);
        std::cout << "Client name: " << clCString_CStr(strPtr) << std::endl;
        clCString_Free(strPtr);
    }

    socketClient = std::make_shared<socket_communication::Client>("127.0.0.1", 5004);

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
