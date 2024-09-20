// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CClient.h"
#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CDeviceInfoList.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Device locator.
 *
 * Searches for available devices of chosen type
 */
CLC_CLASS_WN(DeviceLocatorPrivate, clCDeviceLocator);

/**
 * \brief Device locator error
 */
typedef enum clCDeviceLocatorFailReason {
    clC_DeviceLocatorFailReason_OK = 0,            /**< No error. */
    clC_DeviceLocatorFailReason_BluetoothDisabled, /**< Bluetooth adapter not found or disabled. */
    clC_DeviceLocatorFailReason_Unknown,           /**< Unknown error. */
} clCDeviceLocatorFailReason;

/**
 * \brief Device callback for the list of device info.
 */
CLC_STRUCT(clCDeviceLocatorDelegateDeviceInfoList);
typedef void (*clCDeviceLocatorHandlerDeviceInfoList)(clCDeviceLocator, clCDeviceInfoList, clCDeviceLocatorFailReason);
CL_DLL void clCDeviceLocatorDelegateDeviceInfoList_Set(
    clCDeviceLocatorDelegateDeviceInfoList delegate,
    clCDeviceLocatorHandlerDeviceInfoList callback) NOEXCEPT;

/**
 * Creates locator for Neiry devices of the chosen type.
 *
 * \param client client handle
 * \param deviceType device type
 * \returns device locator handle
 */
CL_DLL clCDeviceLocator clCClient_ChooseDeviceType(clCClient client,
                                                   clCDeviceType deviceType) NOEXCEPT;

/**
 * Release device locator.
 *
 * \param locator device locator handle
 */
CL_DLL void clCDeviceLocator_Destroy(clCDeviceLocator locator) NOEXCEPT;
/**
 * \brief Find available devices. Search time is in seconds.
 * Non-blocking - OnDevices event is fired when timeout expires.
 *
 * \param locator device locator handle
 * \param searchTime search time in seconds
 */
CL_DLL void clCDeviceLocator_RequestDevices(clCDeviceLocator locator,
                                            int32_t searchTime) NOEXCEPT;

/**
 * Get delegate for available devices event.
 *
 * \param locator device locator handle
 * \returns device list delegate
 */
CL_DLL clCDeviceLocatorDelegateDeviceInfoList
clCDeviceLocator_GetOnDevicesEvent(clCDeviceLocator locator) NOEXCEPT;

#ifdef __cplusplus
}
#endif
