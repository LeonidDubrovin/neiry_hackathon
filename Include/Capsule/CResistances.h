// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Used to receive channel resistances from Capsule.
 *
 * Contains electrode resistances of the device
 */
CLC_STRUCT_WN(Resistance, clCResistances);

/**
 * \brief Get total number of resistance channels.
 *
 * \param resistances resistances handle
 * \returns number of channels
 */
CL_DLL int32_t clCResistances_GetCount(clCResistances resistances) NOEXCEPT;
/**
 * \brief Get channel name by index.
 *
 * \param resistances resistances handle
 * \param index channel index
 * \returns channel name
 */
CL_DLL clCString clCResistances_GetChannelName(clCResistances resistances,
                                               int32_t index) NOEXCEPT;
/**
 * \brief Get channel resistance by index.
 *
 * \param resistances resistances handle
 * \param index channel index
 * \returns resistance value
 */
CL_DLL float clCResistances_GetValue(clCResistances resistances, int32_t index) NOEXCEPT;

#ifdef __cplusplus
}
#endif
