// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CPoint3d.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Array of MEMS data with timepoints.
 *
 * Contains samples of accelerometer and gyroscope 3D vectors and their timestamps
 */
CLC_STRUCT_WNN(MEMSTimedData, clCMEMSTimedData, mems);

/**
 * Get total number of MEMS values.
 *
 * \param memsTimedData mems values
 * \returns number of values
 */
CL_DLL int32_t clCMEMSTimedData_GetCount(clCMEMSTimedData memsTimedData) NOEXCEPT;
/**
 * Get accelerometer orientation by index.
 *
 * \param memsTimedData mems values
 * \param index index
 * \returns accelerometer orientation
 */
CL_DLL clCPoint3d clCMEMSTimedData_GetAccelerometer(clCMEMSTimedData memsTimedData,
                                                    int32_t index) NOEXCEPT;
/**
 * Get gyroscope orientation by index.
 *
 * \param memsTimedData mems values
 * \param index index
 * \returns gyroscope orientation
 */
CL_DLL clCPoint3d clCMEMSTimedData_GetGyroscope(clCMEMSTimedData memsTimedData,
                                                int32_t index) NOEXCEPT;
/**
 * Get MEMS time point by index.
 *
 * \param memsTimedData mems values
 * \param index index
 * \returns time point in microseconds
 */
CL_DLL uint64_t clCMEMSTimedData_GetTimepoint(clCMEMSTimedData memsTimedData, int32_t index) NOEXCEPT;

#ifdef __cplusplus
}
#endif
