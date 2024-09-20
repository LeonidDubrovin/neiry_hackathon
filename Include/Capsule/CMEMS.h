// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CMEMSTimedData.h"
#include "Capsule/CNFBCalibrator.h"
#include "Capsule/CSession.h"

#ifdef __cplusplus
extern "C" {
#endif

CLC_CLASS_WN(ClassificationMEMSPrivate, clCMEMS);

/**
 * \brief Create a MEMS classifier.
 *
 * \param session session handle. Valid session must be passed (does not need to be active)
 * \return MEMS classifier handle.
 */
CL_DLL clCMEMS clCMEMS_Create(clCSession session) NOEXCEPT;
/**
 * \brief Create a MEMS classifier after individual NFB has calibrated.
 *
 * \param calibrator NFB calibrator handle. A calibrated calibrator must be passed.
 * \return MEMS handle.
 */
CL_DLL clCMEMS clCMEMS_CreateCalibrated(clCNFBCalibrator calibrator) NOEXCEPT;

/**
 * \brief Initialize cardio classifier
 *
 * \param mems classifier handle
 */
CL_DLL void clCMEMS_Initialize(clCMEMS mems) NOEXCEPT;

CLC_STRUCT(clCMEMSTimedDataDelegate);
/**
 * \brief Get delegate for MEMS data event
 */
CL_DLL clCMEMSTimedDataDelegate clCMEMS_GetOnMEMSTimedDataUpdateEvent(clCMEMS mems) NOEXCEPT;

typedef void (*clCMEMSDataUpdateHandler)(clCMEMS, clCMEMSTimedData);
/**
 * \brief Set handler for MEMS data updates delegate
 */
CL_DLL void clCMEMSDelegateMEMSTimedDataUpdate_Set(clCMEMSTimedDataDelegate delegate,
                                                   clCMEMSDataUpdateHandler callback) NOEXCEPT;

/**
 * \brief Destroy MEMS classifier.
 *
 * \param mems classifier handle.
 */
CL_DLL void clCMEMS_Destroy(clCMEMS mems) NOEXCEPT;

#ifdef __cplusplus
}
#endif
