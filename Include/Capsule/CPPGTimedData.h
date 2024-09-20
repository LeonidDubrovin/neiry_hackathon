// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Array of PPG data with timepoints.
 *
 * Contains samples of PPG sensor and their timestamps
 */

CLC_STRUCT_WNN(PPGTimedData, clCPPGTimedData, cardio);

/**
 * \brief Get total number of PPG values.
 *
 * \param ppgTimedData PPG values
 * \returns number of values
 */
CL_DLL int32_t clCPPGTimedData_GetCount(clCPPGTimedData ppgTimedData) NOEXCEPT;
/**
 * \brief Get PPG value by index.
 *
 * \param ppgTimedData PPG values
 * \param index index
 * \returns ppg value
 */
CL_DLL float clCPPGTimedData_GetValue(clCPPGTimedData ppgTimedData, int32_t index) NOEXCEPT;
/**
 * \brief Get PPG time point by index.
 *
 * \param ppgTimedData PPG values
 * \param index index
 * \returns time point in microseconds
 */
CL_DLL uint64_t clCPPGTimedData_GetTimepoint(clCPPGTimedData ppgTimedData, int32_t index) NOEXCEPT;

#ifdef __cplusplus
}
#endif
