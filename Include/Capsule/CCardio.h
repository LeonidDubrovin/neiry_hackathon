// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CNFBCalibrator.h"
#include "Capsule/CSession.h"

#ifdef __cplusplus
extern "C" {
#endif

CLC_CLASS_WN(ClassificationCardioPrivate, clCCardio);

/**
 * \brief Create a Cardio classifier.
 *
 * \param session session handle. Valid session must be passed (does not need to be active)
 * \return Cardio classifier handle.
 */
CL_DLL clCCardio clCCardio_Create(clCSession session) NOEXCEPT;
/**
 * \brief Create a Cardio classifier after individual NFB has calibrated.
 *
 * \param calibrator NFB calibrator handle. A calibrated calibrator must be passed.
 * \return Cardio handle.
 */
CL_DLL clCCardio clCCardio_CreateCalibrated(clCNFBCalibrator calibrator) NOEXCEPT;

/**
 * \brief Initialize cardio classifier
 *
 * \param cardio classifier handle
 */
CL_DLL void clCCardio_Initialize(clCCardio cardio) NOEXCEPT;

CLC_STRUCT(clCCardioIndexesDelegate);
typedef struct clCCardioData {
    float heartRate;
    float stressIndex;
    float kaplanIndex;
    bool artifacted;
} clCCardioData;
typedef void (*clCCardioIndexesUpdateHandler)(clCCardio, clCCardioData);
/**
 * \brief Get delegate for cardio indexes event
 */
CL_DLL clCCardioIndexesDelegate clCCardio_GetOnIndexesUpdateEvent(clCCardio cardio) NOEXCEPT;
/**
 * \brief Set handler for cardio indexes updates delegate
 */
CL_DLL void clCCardioDelegateIndexesUpdate_Set(clCCardioIndexesDelegate delegate,
                                               clCCardioIndexesUpdateHandler callback) NOEXCEPT;

CLC_STRUCT(clCCardioCalibratedDelegate);
typedef void (*clCCardioCalibratedHandler)(clCCardio);
/**
 * \brief Get delegate for cardio calibrated event
 * Calibration starts automatically when PPG signal starts emitting
 */
CL_DLL clCCardioCalibratedDelegate clCCardio_GetOnCalibratedEvent(clCCardio cardio) NOEXCEPT;
/**
 * \brief Set handler for cardio calibrated delegate
 * Calibration starts automatically when PPG signal starts emitting
 */
CL_DLL void clCCardioDelegateCalibrated_Set(clCCardioCalibratedDelegate delegate,
                                            clCCardioCalibratedHandler handler) NOEXCEPT;

/**
 * \brief Destroy Cardio classifier.
 *
 * \param cardio classifier handle.
 */
CL_DLL void clCCardio_Destroy(clCCardio cardio) NOEXCEPT;

#ifdef __cplusplus
}
#endif
