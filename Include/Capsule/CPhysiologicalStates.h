// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif

CLC_CLASS_WN(ClassificationPhysiologicalStatesPrivate, clCPhysiologicalStates);

typedef struct clCPhysiologicalStatesValue {
    float relaxation;
    float fatigue;
    float none;
    float concentration;
    float involvement;
    float stress;
    bool nfbArtifacts;
    bool cardioArtifacts;
} clCPhysiologicalStatesValue;

/**
 * \brief Create a PhysiologicalStates classifier.
 *
 * \param session session handle. Valid session must be passed (does not need to be active)
 * \return PhysiologicalStates classifier handle.
 */
CL_DLL clCPhysiologicalStates clCPhysiologicalStates_Create(clCSession session) NOEXCEPT;

/**
 * \brief Destroy PhysiologicalStates classifier.
 *
 * \param states classifier handle.
 */
CL_DLL void clCPhysiologicalStates_Destroy(clCPhysiologicalStates states) NOEXCEPT;

CLC_STRUCT(clCPhysiologicalStatesDelegatePhysiologicalStatesUpdate);
typedef void (*clCPhysiologicalStatesHandlerPhysiologicalStatesUpdate)(clCPhysiologicalStates, const clCPhysiologicalStatesValue*);
/**
 * \brief Set handler for physiological states update event
 */
CL_DLL void clCPhysiologicalStatesOnPhysiologicalStatesUpdateEvent_Set(clCPhysiologicalStatesDelegatePhysiologicalStatesUpdate delegate,
                                                                       clCPhysiologicalStatesHandlerPhysiologicalStatesUpdate handler) NOEXCEPT;
/**
 * \brief Set delegate for physiological states update event
 */
CL_DLL clCPhysiologicalStatesDelegatePhysiologicalStatesUpdate clCPhysiologicalStates_GetOnPhysiologicalStatesUpdateEvent(clCPhysiologicalStates states) NOEXCEPT;

CLC_STRUCT(clCPhysiologicalStatesDelegateInitialized);
typedef void (*clCPhysiologicalStatesHandlerInitialized)(clCPhysiologicalStates);
/**
 * \brief Set handler for physiological staes initialized delegate
 */
CL_DLL void clCPhysiologicalStatesOnInitializedEvent_Set(clCPhysiologicalStatesDelegateInitialized delegate, clCPhysiologicalStatesHandlerInitialized handler) NOEXCEPT;
/**
 * \brief Get delegate for physiological states initialized event
 */
CL_DLL clCPhysiologicalStatesDelegateInitialized clCPhysiologicalStates_GetOnInitializedEvent(clCPhysiologicalStates states) NOEXCEPT;

typedef struct clCPhysiologicalStatesBaselines {
    float alpha;
    float beta;
    float alphaGravity;
    float betaGravity;
    float concentration;
} clCPhysiologicalStatesBaselines;

CLC_STRUCT(clCPhysiologicalStatesDelegateCalibrated);
typedef void (*clCPhysiologicalStatesHandlerCalibrated)(clCPhysiologicalStates, const clCPhysiologicalStatesBaselines*);

/**
 * \brief Set handler for physiological staes calibrated delegate
 */
CL_DLL void clCPhysiologicalStatesOnCalibratedEvent_Set(clCPhysiologicalStatesDelegateCalibrated delegate,
                                                        clCPhysiologicalStatesHandlerCalibrated handler) NOEXCEPT;
/**
 * \brief Get delegate for physiological states calibrated event
 */
CL_DLL clCPhysiologicalStatesDelegateCalibrated clCPhysiologicalStates_GetOnCalibratedEvent(clCPhysiologicalStates states) NOEXCEPT;

/**
 * \brief Initialize physiological states classifier with default calculation period
 *
 * \param states classifier handle
 */
CL_DLL void clCPhysiologicalStates_Initialize(clCPhysiologicalStates states) NOEXCEPT;

/**
 * \brief Initialize physiological states classifier with custom calculation period
 *
 * \param states classifier handle
 * \param calculationPeriod number of seconds between states calculations
 */
CL_DLL void clCPhysiologicalStates_InitializeWithCalculationPeriod(clCPhysiologicalStates states, float calculationPeriod) NOEXCEPT;

/**
 * \brief Start physiological states calibration
 *
 * \param states classifier handle
 */
CL_DLL void clCPhysiologicalStates_StartCalibration(clCPhysiologicalStates states) NOEXCEPT;

#ifdef __cplusplus
}
#endif
