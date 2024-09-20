// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CString.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Neiry Device type.
 */
typedef enum clCDeviceType {
    clC_DT_NeiryBand = 0,
    clC_DT_NeiryBuds = 1,
    clC_DT_NeiryHeadphones = 2,
    clC_DT_NeiryImpulse = 3,
    clC_DT_NeiryAny = 4,
    clC_DT_BrainBit2 = 6,
    clC_DT_DevOnly_SinWave = 100,
    clC_DT_DevOnly_Noise = 101,
    clC_DT_DevOnly_LSL,
    clC_DT_DevOnly_File
} clCDeviceType;

/**
 * \brief Device info list.
 *
 * Contains device information: name, id (serial number) and type
 */
CLC_STRUCT_WNN(DeviceInfo, clCDeviceInfo, device);

/**
 * Get device ID (serial number).
 *
 * \param device device info handle
 * \returns device ID string
 */
CL_DLL clCString clCDeviceInfo_GetID(clCDeviceInfo device) NOEXCEPT;
/**
 * Get device name.
 *
 * \param device device info handle
 * \returns device name string
 */
CL_DLL clCString clCDeviceInfo_GetName(clCDeviceInfo device) NOEXCEPT;
/**
 * Get device type.
 *
 * \param device device info handle
 * \returns device type
 */
CL_DLL clCDeviceType clCDeviceInfo_GetType(clCDeviceInfo device) NOEXCEPT;
/**
 * Get compound device description string.
 *
 * \param device device info handle
 * \returns device description string
 */
CL_DLL clCString clCDeviceInfo_GetDescription(clCDeviceInfo device) NOEXCEPT;

#ifdef __cplusplus
}
#endif
