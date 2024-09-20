// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif

CLC_CLASS_WN(NFBBaselinePrivate, clCNFBBaseline);
/**
 * \brief Creates clCNFBBaseline handle - buffer for baseline calculation.
 * \param[in] windowsSizeSec window size (in seconds).
 * \param[in] strideSec stride size (for a second).
 *
 * \returns \ref clCNFBBaseline handle, that needs to be destroyed with
 * \ref clCNFBBaseline_Destroy.
 */
CL_DLL clCNFBBaseline clCNFBBaseline_Create(float windowSizeSec,
                                            float strideSec) NOEXCEPT;
/**
 * \brief Destroy \ref clCNFBBaseline handle.
 * \param[in,out] baseline handle.
 */
CL_DLL void clCNFBBaseline_Destroy(clCNFBBaseline baseline) NOEXCEPT;

/**
 * \brief Push (time marked) data to clCNFBBaseline buffer.
 * \param[in,out] baseline handle.
 * \param[in] nfb nfb value to insert.
 * \param[in] timestamp timestamp mark of inserted \ref value.
 */
CL_DLL void clCNFBBaseline_PushNFB(clCNFBBaseline baseline, float nfb,
                                   int64_t timestamp) NOEXCEPT;

/**
 * \brief Get value from \ref clCNFBBaseline for given percentile.
 * \param[in,out] baseline handle.
 * \param[in] percentile percentile to calculate value
 * \returns percentile for the handle
 */
CL_DLL float clCNFBBaseline_Get(clCNFBBaseline baseline, uint8_t percentile) NOEXCEPT;

#ifdef __cplusplus
}
#endif
