// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CDeviceInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief List of devices found by device locator.
 *
 * Contains information about the devices
 */
CLC_STRUCT(clCDeviceInfoList);

/**
 * Get the number of devices.
 *
 * \param devices device list handle
 * \returns count of device list
 */
CL_DLL int32_t clCDeviceInfoList_GetCount(clCDeviceInfoList devices) NOEXCEPT;
/**
 * Get device info from the list.
 *
 * \param devices device list handle
 * \param index info index
 * \returns device info handle
 */
CL_DLL clCDeviceInfo clCDeviceInfoList_GetDeviceInfo(clCDeviceInfoList devices,
                                                     int32_t index) NOEXCEPT;

#ifdef __cplusplus
}
#endif
