// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CSession.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Individual NFB calibrator
 */
CLC_CLASS_WN(NFBCalibratorPrivate, clCNFBCalibrator);

/**
 * \brief Obtain an individual NFB calibrator for the session. If not present, it'll
 * be created.
 *
 * \param session session handle. A valid session must be passed (does not need to
 * be active)
 *
 * \returns NFB calibrator handle
 */
CL_DLL clCNFBCalibrator clCNFBCalibrator_CreateOrGet(clCSession session) NOEXCEPT;

typedef enum clCIndividualNFBCalibrationStage {
    clCIndividualNFBCalibrationStage1,
    clCIndividualNFBCalibrationStage2,
    clCIndividualNFBCalibrationStage3,
    clCIndividualNFBCalibrationStage4,
} clCIndividualNFBCalibrationStage;
/**
 * \brief Start stage of individual NFB calibration
 *
 * \param stage the user must call this method 4 times sequentially
 * changing this argument from Stage1 to Stage4
 * \param error the error out parameter
 */
CL_DLL void clCNFBCalibrator_CalibrateIndividualNFB(clCNFBCalibrator nfbCalibrator,
                                                    clCIndividualNFBCalibrationStage stage,
                                                    clCError* error) NOEXCEPT;

/**
 * \brief Start stage of quick individual NFB calibration
 *
 * \param error the error out parameter
 */
CL_DLL void clCNFBCalibrator_CalibrateIndividualNFBQuick(clCNFBCalibrator nfbCalibratorPtr,
                                                         clCError* error) NOEXCEPT;

CLC_STRUCT(clCNFBCalibratorDelegateIndividualNFBCalibrationStage);
typedef void (*clCNFBCalibratorHandlerIndividualNFBCalibrationStage)(clCNFBCalibrator);
/**
 * \brief Set handler for individual calibration event
 */
CL_DLL void clCNFBCalibratorDelegateIndividualNFBCalibrationStage_Set(
    clCNFBCalibratorDelegateIndividualNFBCalibrationStage delegate,
    clCNFBCalibratorHandlerIndividualNFBCalibrationStage handler) NOEXCEPT;
/**
 * \brief Get handler for individual calibration stage event
 *
 * \param nfbCalibrator
 * \return calibration stage delegate
 */
CL_DLL clCNFBCalibratorDelegateIndividualNFBCalibrationStage
clCNFBCalibrator_GetOnIndividualNFBStageFinishedEvent(clCNFBCalibrator nfbCalibrator) NOEXCEPT;

struct clCIndividualNFBData {
    /**
     * \brief Individual NFB frequency.
     */
    float individualFrequency = 0.0f;
    /**
     * \brief Individual NFB peak frequency.
     */
    float individualPeakFrequency = 0.0f;
    /**
     * \brief Individual NFB peak frequency power.
     */
    float individualPeakFrequencyPower = 0.0f;
    /**
     * \brief Individual NFB peak frequency suppression.
     */
    float individualPeakFrequencySuppression = 0.0f;
    /**
     * \brief Individual NFB bandwidth.
     */
    float individualBandwidth = 0.0f;
    /**
     * \brief NFB power.
     */
    float individualNormalizedPower = 0.0f;
    /**
     * \brief Left frequency bound
     */
    float lowerFrequency = 0.0f;
    /**
     * \brief Right frequency bound
     */
    float upperFrequency = 0.0f;
};

typedef enum clCIndividualNFBCalibrationFailReason {
    clC_IndividualNFBCalibrationFailReason_None = 0,
    clC_IndividualNFBCalibrationFailReason_TooManyArtifacts,
    clC_IndividualNFBCalibrationFailReason_PeakIsABorder,
} clCIndividualNFBCalibrationFailReason;

CLC_STRUCT(clCNFBCalibratorDelegateIndividualNFBCalibrated);
typedef void (*clCNFBCalibratorHandlerIndividualNFBCalibrated)(
    clCNFBCalibrator, const clCIndividualNFBData*, clCIndividualNFBCalibrationFailReason);
/**
 * \brief Set handler for individual calibration finish event
 */
CL_DLL void clCNFBCalibratorDelegateIndividualNFBCalibrated_Set(
    clCNFBCalibratorDelegateIndividualNFBCalibrated delegate,
    clCNFBCalibratorHandlerIndividualNFBCalibrated handler) NOEXCEPT;
/**
 * \brief Get handler for individual calibration finish event
 *
 * \param nfbCalibrator
 * \return calibration finish delegate
 */
CL_DLL clCNFBCalibratorDelegateIndividualNFBCalibrated
clCNFBCalibrator_GetOnIndividualNFBCalibratedEvent(clCNFBCalibrator nfbCalibrator) NOEXCEPT;

/**
 * \brief Is calibration finished
 *
 * \param nfbCalibrator
 * \return \c true if calibration has finished
 */
CL_DLL bool clCNFBCalibrator_IsCalibrated(clCNFBCalibrator nfbCalibrator) NOEXCEPT;
/**
 * \brief Has calibration failed
 *
 * \param nfbCalibrator
 * \return \c true if calibration has failed
 */
CL_DLL bool clCNFBCalibrator_HasCalibrationFailed(clCNFBCalibrator nfbCalibrator) NOEXCEPT;

/**
 * \brief Is calibrator ready to calibrate individual NFB data
 *
 * \param nfbCalibrator
 * \return \c true if calibrator is ready to calibrate individual NFB data
 */
CL_DLL bool clCNFBCalibrator_IsReady(clCNFBCalibrator nfbCalibrator) NOEXCEPT;

CLC_STRUCT(clCNFBCalibratorDelegateReadyToCalibrate);
typedef void (*clCNFBCalibratorHandlerReadyToCalibrate)(clCNFBCalibrator);
/**
 * \brief Set handler for calibrator ready to calibrate
 */
CL_DLL void clCNFBCalibratorDelegateReadyToCalibrate_Set(
    clCNFBCalibratorDelegateReadyToCalibrate delegate,
    clCNFBCalibratorHandlerReadyToCalibrate handler) NOEXCEPT;
/**
 * \brief Get handler for calibrator ready to calibrate
 *
 * \param nfbCalibrator
 * \return calibrator ready to calibrate delegate
 */
CL_DLL clCNFBCalibratorDelegateReadyToCalibrate
clCNFBCalibrator_GetOnReadyToCalibrate(clCNFBCalibrator nfbCalibrator) NOEXCEPT;

#ifdef __cplusplus
}
#endif
