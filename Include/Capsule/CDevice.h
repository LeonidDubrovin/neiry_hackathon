// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CDeviceLocator.h"
#include "Capsule/CEEGTimedData.h"
#include "Capsule/CMEMSTimedData.h"
#include "Capsule/CPPGTimedData.h"
#include "Capsule/CResistances.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Neiry Device.
 *
 * Controls connection to the device
 */
CLC_CLASS_WN(DevicePrivate, clCDevice);

typedef enum clCDeviceMode {
    clC_DM_Resistance,
    clC_DM_Idle,
    clC_DM_PowerDown,
    clC_DM_Signal,
    clC_DM_StartMEMS,
    clC_DM_StopMEMS,
    clC_DM_StartPPG,
    clC_DM_StopPPG,
    clC_DM_SignalAndResist
} clCDeviceMode;

/**
 * \brief Device connection state.
 */
typedef enum clCDeviceConnectionState {
    clC_SE_Disconnected = 0,
    clC_SE_Connected = 1,
    clC_SE_UnsupportedConnection = 2,
} clCDeviceConnectionState;

/**
 * Create device for interaction.
 *
 * \param locator device locator handle
 * \param deviceID device ID string
 * \returns device handle
 */
CL_DLL clCDevice clCDeviceLocator_CreateDevice(clCDeviceLocator locator, const char* deviceID) NOEXCEPT;

/**
 * Device delegate without parameters. Not supported in the current version
 */
CLC_STRUCT(clCDeviceDelegate);
typedef void (*clCDeviceHandler)(clCDevice);
CL_DLL void clCDeviceDelegate_Set(clCDeviceDelegate delegate,
                                  clCDeviceHandler callback) NOEXCEPT;

/**
 * Device delegate for resistances event.
 */
CLC_STRUCT(clCDeviceDelegateResistances);
typedef void (*clCDeviceHandlerResistances)(clCDevice, clCResistances);
CL_DLL void
clCDeviceDelegateResistances_Set(clCDeviceDelegateResistances delegate,
                                 clCDeviceHandlerResistances callback) NOEXCEPT;
/**
 * Device delegate for MEMS data event.
 */
CLC_STRUCT(clCDeviceDelegateMEMSData);
typedef void (*clCDeviceHandlerMEMSData)(clCDevice, clCMEMSTimedData);
CL_DLL void
clCDeviceDelegateMEMSData_Set(clCDeviceDelegateMEMSData delegate,
                              clCDeviceHandlerMEMSData callback) NOEXCEPT;
/**
 * Device delegate for PPG data event.
 */
CLC_STRUCT(clCDeviceDelegatePPGData);
typedef void (*clCDeviceHandlerPPGData)(clCDevice, clCPPGTimedData);
CL_DLL void
clCDeviceDelegatePPGData_Set(clCDeviceDelegatePPGData delegate,
                             clCDeviceHandlerPPGData callback) NOEXCEPT;
/**
 * Device delegate for EEG data event.
 */
CLC_STRUCT(clCDeviceDelegateEEGData);
typedef void (*clCDeviceHandlerEEGData)(clCDevice, clCEEGTimedData);
CL_DLL void
clCDeviceDelegateEEGData_Set(clCDeviceDelegateEEGData delegate,
                             clCDeviceHandlerEEGData callback) NOEXCEPT;
/**
 * Device delegate for battery charge.
 */
typedef unsigned char clCUChar;
CLC_STRUCT(clCDeviceDelegateUChar);
typedef void (*clCDeviceHandlerUChar)(clCDevice, clCUChar);
CL_DLL void clCDeviceDelegateBatteryCharge_Set(clCDeviceDelegateUChar delegate,
                                               clCDeviceHandlerUChar callback) NOEXCEPT;

CLC_STRUCT(clCDeviceDelegateMode);
typedef void (*clCDeviceHandlerMode)(clCDevice, clCDeviceMode);
CL_DLL void clCDeviceDelegateMode_Set(clCDeviceDelegateMode delegate,
                                      clCDeviceHandlerMode callback) NOEXCEPT;
/**
 * Get delegate for device connection state event.
 *
 * \param device device handle
 */
CLC_STRUCT(clCDeviceDelegateDeviceConnectionState);
typedef void (*clCDeviceHandlerDeviceConnectionState)(clCDevice, clCDeviceConnectionState);
CL_DLL clCDeviceDelegateDeviceConnectionState
clCDevice_GetOnConnectionStateChangedEvent(clCDevice device) NOEXCEPT;

/**
 * Device connection delegate.
 */
CLC_STRUCT(clCDeviceDelegateConnectionState);
typedef void (*clCDeviceHandlerConnectionState)(clCDevice, clCDeviceConnectionState);
CL_DLL void clCDeviceDelegateConnectionState_Set(clCDeviceDelegateDeviceConnectionState delegate,
                                                 clCDeviceHandlerConnectionState callback) NOEXCEPT;

/**
 * Connect to Neiry device. Non-blocking - OnConnectionStateChanged event is
 * fired when connected.
 *
 * \param device device handle
 */
CL_DLL void clCDevice_Connect(clCDevice device) NOEXCEPT;
/**
 * Disconnect from Neiry device. Non-blocking - OnConnectionStateChanged event
 * is fired when disconnected.
 *
 * \param device device handle
 */
CL_DLL void clCDevice_Disconnect(clCDevice device) NOEXCEPT;
/**
 * \brief Get device battery charge
 *
 * \param device
 * \return battery charge
 */
CL_DLL clCUChar clCDevice_GetBatteryCharge(clCDevice device) NOEXCEPT;
/**
 * \brief Check if capsule has read firmware version
 * \param device Device handle
 * \return true if capsule has read firmware version, false otherwise
 */
CL_DLL bool clCDevice_FirmwareVersionReceived(clCDevice device) NOEXCEPT;
/**
 * \brief Get device's firmware version
 * \param device Device handle
 * \param error Pointer to int value, which sets error code (0 is a success)
 * \return Firmware version string
 */
CL_DLL clCString clCDevice_GetFirmwareVersion(clCDevice device, clCError* error) NOEXCEPT;
/**
 * \brief Switch device from signal to resistance mode or switch the device off
 *
 * \param device
 * \param mode
 */
CL_DLL void clCDevice_SwitchMode(clCDevice device, clCDeviceMode mode) NOEXCEPT;
CL_DLL clCDeviceMode clCDevice_GetMode(clCDevice device) NOEXCEPT;

/**
 * Is Neiry device connected to Capsule.
 *
 * \param device device handle
 */
CL_DLL bool clCDevice_IsConnected(clCDevice device) NOEXCEPT;
/**
 * Get device information.
 *
 * \param device device handle
 * \returns device info handle
 */
CL_DLL clCDeviceInfo clCDevice_GetInfo(clCDevice device) NOEXCEPT;
/**
 * Get names of channels of the device
 *
 * \param device device handle
 */
CLC_STRUCT_WN(DeviceChannelNames, clCDeviceChannelNames);
CL_DLL clCDeviceChannelNames clCDevice_GetChannelNames(clCDevice device) NOEXCEPT;

CL_DLL int32_t clCDevice_GetChannelsCount(clCDeviceChannelNames deviceChannels) NOEXCEPT;

CL_DLL int32_t clCDevice_GetChannelIndexByName(clCDeviceChannelNames deviceChannels,
                                               const char* channelName) NOEXCEPT;

CL_DLL clCString clCDevice_GetChannelNameByIndex(clCDeviceChannelNames deviceChannels,
                                                 int32_t channelIndex) NOEXCEPT;
/**
 * Get delegate for device resistances event.
 *
 * \param device device handle
 */
CL_DLL clCDeviceDelegateResistances
clCDevice_GetOnResistancesEvent(clCDevice device) NOEXCEPT;
/**
 * Get delegate for device MEMS data event.
 *
 * \param device device handle
 */
CL_DLL clCDeviceDelegateMEMSData
clCDevice_GetOnMEMSDataEvent(clCDevice device) NOEXCEPT;
/**
 * Get delegate for device PPG data event.
 *
 * \param device device handle
 */
CL_DLL clCDeviceDelegatePPGData
clCDevice_GetOnPPGDataEvent(clCDevice device) NOEXCEPT;

/**
 * Get delegate for device EEG data event.
 *
 * \param device device handle
 */
CL_DLL clCDeviceDelegateEEGData
clCDevice_GetOnEEGDataEvent(clCDevice device) NOEXCEPT;

/**
 * Get delegate for device battery charge event.
 *
 * \param device device handle
 */
CL_DLL clCDeviceDelegateUChar clCDevice_GetOnBatteryChargeEvent(clCDevice device) NOEXCEPT;
CL_DLL clCDeviceDelegateMode clCDevice_GetOnModeSwitchedEvent(clCDevice device) NOEXCEPT;
/**
 * Release the device.
 *
 * \param device device handle
 */
CL_DLL void clCDevice_Release(clCDevice device) NOEXCEPT;

#ifdef __cplusplus
}
#endif
