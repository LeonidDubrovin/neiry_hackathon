// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CError.h"
#include "Capsule/CNFBCalibrator.h"
#include "Capsule/CSession.h"

#ifdef __cplusplus
extern "C" {
#endif

CLC_CLASS_WN(ProductivityMetricsPrivate, clCNFBMetricProductivity);

/**
 * \brief Create a NFB classifier for metrics productivity
 *
 * \param session session handle. Valid session must be passed (does not need to
 * be active)
 * \param path path to the directory with logs
 * \param error out error parameter
 *
 * \returns NFBMetrics productivity handle
 */
CL_DLL clCNFBMetricProductivity clCNFBMetricsProductivity_Create(
    clCSession session, const char* path, double speed, double maxSpeed,
    double slowDown, clCError* error) NOEXCEPT;
/**
 * \brief Create a NFB classifier for metrics productivity after individual NFB has been calibrated
 *
 * \param calibrator calibrator handle. A calibrated calibrator must be passed
 * \param path path to the directory with logs
 * \param error out error parameter
 *
 * \returns NFBMetrics productivity handle
 */
CL_DLL clCNFBMetricProductivity clCNFBMetricsProductivity_CreateCalibrated(
    clCNFBCalibrator calibrator, const char* path, double speed, double maxSpeed,
    double slowDown, clCError* error) NOEXCEPT;
/**
 * \brief Destroy NFB classifier. Release the NFBMetrics productivity handle.
 *
 * \param nfbMetrics NFBMetrics productivity handle
 */
CL_DLL void
clCNFBMetricsProductivity_Destroy(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Initialize NFB classifier for metrics productivity
 *
 * \param nfbMetrics NFBMetrics productivity handle
 */
CL_DLL void
clCNFBMetricsProductivity_InitializeNFB(clCNFBMetricProductivity nfbMetrics,
                                        const char* platformAddress,
                                        clCError* error) NOEXCEPT;

CLC_STRUCT(clCNFBMetricsProductivityCalibratedDelegate);
typedef void (*clCNFBMetricsProductivityCalibratedHandler)(
    clCNFBMetricProductivity);
/**
 * \brief Set handler for calibrated event NFB classifier, not involved yet
 */
CL_DLL void clCNFBMetricsProductivity_CalibratedEvent_Set(
    clCNFBMetricsProductivityCalibratedDelegate delegate,
    clCNFBMetricsProductivityCalibratedHandler handler) NOEXCEPT;

/**
 * \brief Get delegate for baseline calibration finished event
 */
CL_DLL clCNFBMetricsProductivityCalibratedDelegate
clCNFBMetricsProductivity_GetOnBaselineCalibratedEvent(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;

typedef struct clCNFBMetricsProductivityBaselines {
    float gravityBaseline;
    float productivityBaseline;
    float fatigueBaseline;
    float reverseFatigueBaseline;
    float relaxBaseline;
    float concentrationBaseline;
} clCNFBMetricsProductivityBaselines;
CLC_STRUCT(clCNFBMetricsProductivityInitialBaselineCalibratedDelegate);
typedef void (*clCNFBMetricsProductivityInitialBaselineCalibratedHandler)(
    clCNFBMetricProductivity, const clCNFBMetricsProductivityBaselines*);
/**
 * \brief Set delegate for initial baseline calibration finished event
 */
CL_DLL void clCNFBMetricsProductivity_InitialBaselineCalibratedEvent_Set(
    clCNFBMetricsProductivityInitialBaselineCalibratedDelegate delegate,
    clCNFBMetricsProductivityInitialBaselineCalibratedHandler handler) NOEXCEPT;

/**
 * \brief Get delegate for initial baseline calibration finished event
 */
CL_DLL clCNFBMetricsProductivityInitialBaselineCalibratedDelegate
clCNFBMetricsProductivity_GetOnInitialBaselineCalibratedEvent(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;

typedef struct clCNFBMetricsProductivityIndividualIndexes {
    float IAF;
    float IAPF;
    float IAPFPowerNormalized;
    float alphaRangeBegin;
    float alphaRangeEnd;
    uint8_t relaxPoint;
    uint8_t stressPoint;
    float relaxBaseline;
    float fatigueBaseline;
    float gravityBaseline;
    float concentrationBaseline;
    int isArtifacted;
} clCNFBMetricsProductivityIndividualIndexes;

CLC_STRUCT(clCNFBMetricsProductivityIndividualDelegate);
typedef void (*clCNFBMetricsProductivityIndividualHandler)(clCNFBMetricProductivity,
                                                           const clCNFBMetricsProductivityIndividualIndexes*);

/**
 * \brief Get delegate for Individual measure event
 */
CL_DLL clCNFBMetricsProductivityIndividualDelegate
clCNFBMetricsProductivity_GetOnIndividualMeasuredEvent(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Set handler for Individual measure event
 */
CL_DLL void clCNFBMetricsProductivity_IndividualMeasuredEvent_Set(
    clCNFBMetricsProductivityIndividualDelegate delegate,
    clCNFBMetricsProductivityIndividualHandler handler) NOEXCEPT;

typedef struct clCNFBMetricsProductivityValues {
    float fatigueScore;
    float gravityScore;
    float concentrationScore;
    float relaxationScore;
    float accumulatedFatigue;
    int32_t fatigueGrowthRate;
} clCNFBMetricsProductivityValues;

CLC_STRUCT(clCNFBMetricsProductivityValuesDelegate);
typedef void (*clCNFBMetricsProductivityValuesHandler)(clCNFBMetricProductivity,
                                                       const clCNFBMetricsProductivityValues*);
/**
 * \brief Get delegate for Productivity scores event
 */
CL_DLL clCNFBMetricsProductivityValuesDelegate
clCNFBMetricsProductivity_GetOnProductivityValuesEvent(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Set handler for Productivity scores event
 */
CL_DLL void clCNFBMetricsProductivity_ValuesEvent_Set(
    clCNFBMetricsProductivityValuesDelegate delegate,
    clCNFBMetricsProductivityValuesHandler handler) NOEXCEPT;

/**
 * \brief Set handler for Individual measure event
 *
 * \returns IAPF or 0 if until the measurement individual state is completed
 */
CL_DLL float clCNFBMetricsProductivity_GetIAPF(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Set handler for Individual measure event
 *
 * \returns IAF or 0 if until the measurement individual state is completed
 */
CL_DLL float
clCNFBMetricsProductivity_GetIAF(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Reset accumulated fatigue and start calculated it from 0
 */
CL_DLL void clCNFBMetricsProductivity_ResetAccumulatedFatigue(clCNFBMetricProductivity nfbMetrics,
                                                              clCError* error) NOEXCEPT;
/**
 * \brief Artifacts when measuring the state user
 */
typedef struct clCNFBUserArtifacts {
    /**
     * \brief Artifacts from channel.
     */
    const uint8_t* artifactsData;
    uint64_t artifactsSize;
    int64_t timestamp;
} clCNFBUserArtifacts;

CLC_STRUCT(clCNFBMetricsProductivityArtifactsDelegate);
typedef void (*clCNFBMetricsProductivityArtifactsHandler)(
    clCNFBMetricProductivity, const clCNFBUserArtifacts*);
/**
 * \brief Get delegate for occurrence artifacts event
 */
CL_DLL clCNFBMetricsProductivityArtifactsDelegate
clCNFBMetricsProductivity_GetOnArtifactsEvent(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Set handler for occurrence artifacts event
 */
CL_DLL void clCNFBMetricsProductivity_ArtifactsEvent_Set(
    clCNFBMetricsProductivityArtifactsDelegate delegate,
    clCNFBMetricsProductivityArtifactsHandler handler) NOEXCEPT;

//------------------
typedef float clCNFBMetricsProductivity_CurrentPowerIAPF,
    clCNFBMetricsProductivity_relaxValue;

CLC_STRUCT(clCNFBMetricsProductivityTrainingInfoDelegate);
typedef void (*clCNFBMetricsProductivityTrainingInfoHandler)(
    clCNFBMetricProductivity, clCNFBMetricsProductivity_CurrentPowerIAPF,
    clCNFBMetricsProductivity_relaxValue);

CL_DLL void clCNFBMetricsProductivity_TrainingInfoEvent_Set(
    clCNFBMetricsProductivityTrainingInfoDelegate delegate,
    clCNFBMetricsProductivityTrainingInfoHandler handler) NOEXCEPT;

CL_DLL clCNFBMetricsProductivityTrainingInfoDelegate
clCNFBMetricsProductivity_GetOnTrainingInfoEvent(clCNFBMetricProductivity nfbMetrics) NOEXCEPT;

//----------------

CLC_STRUCT(clCNFBMetricsProductivityEventDelegate);
typedef void (*clCNFBMetricsProductivityEventHandler)(clCNFBMetricProductivity, float);
/**
 * \brief Set handler for update event productivity metrics: 1min, 5min ...
 * 60min
 */
CL_DLL void clCNFBMetricsProductivity_UpdateEvent_Set(
    clCNFBMetricsProductivityEventDelegate delegate,
    clCNFBMetricsProductivityEventHandler handler) NOEXCEPT;
/**
 * \brief Get delegate for update event productivity metric 1 min.
 */
CL_DLL clCNFBMetricsProductivityEventDelegate
clCNFBMetricsProductivity_GetOnUpdateEvent_1min(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Get delegate for update event productivity metric 5 min.
 */
CL_DLL clCNFBMetricsProductivityEventDelegate
clCNFBMetricsProductivity_GetOnUpdateEvent_5min(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Get delegate for update event productivity metric 15 min.
 */
CL_DLL clCNFBMetricsProductivityEventDelegate
clCNFBMetricsProductivity_GetOnUpdateEvent_15min(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Get delegate for update event productivity metric 30 min.
 */
CL_DLL clCNFBMetricsProductivityEventDelegate
clCNFBMetricsProductivity_GetOnUpdateEvent_30min(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;
/**
 * \brief Get delegate for update event productivity metric 60 min.
 */
CL_DLL clCNFBMetricsProductivityEventDelegate
clCNFBMetricsProductivity_GetOnUpdateEvent_60min(
    clCNFBMetricProductivity nfbMetrics) NOEXCEPT;

#ifdef __cplusplus
}
#endif
