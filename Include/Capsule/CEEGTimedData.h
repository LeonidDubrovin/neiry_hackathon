// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Array of EEG data with timepoints.
 *
 * Contains vector of EEG channels and timestamps
 */

CLC_STRUCT_WN(EEGTimedData, clCEEGTimedData);

/**
 * Get number of EEG channels.
 *
 * \param eegTimedData EEG values
 * \returns number of channels
 */
CL_DLL int32_t clCEEGTimedData_GetChannelsCount(clCEEGTimedData eegTimedData) NOEXCEPT;
/**
 * Get number of EEG samples in a channel.
 *
 * \param eegTimedData EEG values
 * \returns number of samples in a channel
 */
CL_DLL int32_t clCEEGTimedData_GetSamplesCount(clCEEGTimedData eegTimedData) NOEXCEPT;
/**
 * Get EEG value by channel index and sample index.
 *
 * \param eegTimedData eeg timed values
 * \param channelIndex index of a channel
 * \param sampleIndex index of a sample in a channel
 * \returns eeg value
 */
CL_DLL float clCEEGTimedData_GetValue(clCEEGTimedData eegTimedData, int32_t channelIndex,
                                      int32_t sampleIndex) NOEXCEPT;
/**
 * Get EEG time point by index.
 *
 * \param eegTimedData EEG values
 * \param index index
 * \returns time point in microseconds
 */
CL_DLL uint64_t clCEEGTimedData_GetTimepoint(clCEEGTimedData eegTimedData, int32_t index) NOEXCEPT;

#ifdef __cplusplus
}
#endif
