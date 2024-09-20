// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Wrapper of \ref clCNFB
 *
 * Use it for metrics described in \ref clCEmotionsEmotion enumeration. **Do
 * not use it at the same time as using \ref clCNFB.
 */
CLC_CLASS_WN(ClassificationEmotionsPrivate, clCEmotions);

/**
 * \brief Metrics you can compute with \ref clCEmotions
 */
typedef enum clCEmotionsEmotion {
    clC_Emotions_EmotionFocus,
    clC_Emotions_EmotionChill,
    clC_Emotions_EmotionStress,
    clC_Emotions_EmotionAnger,
    clC_Emotions_EmotionSelfControl,
} clCEmotionsEmotion;

/**
 * \brief Emotions values, determined by Emotions classifier.
 */
typedef struct clCEmotionalStates {
    float focus;
    float chill;
    float stress;
    float anger;
    float selfControl;
} clCEmotionalStates;

/**
 * Create an Emotions classifier.
 * Start calibration and update metrics after NFB calibration finished.
 *
 * \param session session handle. Valid session must be passed (does not need to be active)
 * \return Emotions handle.
 */
CL_DLL clCEmotions clCEmotions_Create(clCSession session, double speed, double maxSpeed) NOEXCEPT;
/**
 * Create an Emotions classifier after individual NFB has calibrated.
 *
 * \param calibrator NFB calibrator handle. A calibrated calibrator must be passed.
 * \return Emotions handle.
 */
CL_DLL clCEmotions clCEmotions_CreateCalibrated(clCNFBCalibrator calibrator, double speed,
                                                double maxSpeed) NOEXCEPT;
/**
 * Destroy Emotions classifier.
 *
 * \param emotions Emotions handle.
 */
CL_DLL void clCEmotions_Destroy(clCEmotions emotions) NOEXCEPT;

CLC_STRUCT(clCEmotionsDelegateFloat);
typedef void (*clCEmotionsHandlerFloat)(clCEmotions, float);
CL_DLL void clCEmotionsDelegateFloat_Set(clCEmotionsDelegateFloat delegate,
                                         clCEmotionsHandlerFloat handler) NOEXCEPT;
CL_DLL clCEmotionsDelegateFloat clCEmotions_GetOnEmotionUpdateEvent(
    clCEmotions emotions, clCEmotionsEmotion emotion) NOEXCEPT;

CLC_STRUCT(clCEmotionsDelegateEmotionalStatesUpdate);
typedef void (*clCEmotionsHandlerEmotionalStatesUpdate)(clCEmotions, const clCEmotionalStates*);
CL_DLL void clCEmotionsDelegateEmotionalStatesUpdate_Set(
    clCEmotionsDelegateEmotionalStatesUpdate delegate,
    clCEmotionsHandlerEmotionalStatesUpdate handler) NOEXCEPT;
CL_DLL clCEmotionsDelegateEmotionalStatesUpdate
clCEmotions_GetOnEmotionalStatesUpdateEvent(clCEmotions emotions) NOEXCEPT;

CLC_STRUCT(clCEmotionsDelegate);
typedef void (*clCEmotionsHandler)(clCEmotions);
CL_DLL void clCEmotionsDelegate_Set(clCEmotionsDelegate delegate, clCEmotionsHandler handler) NOEXCEPT;
CL_DLL clCEmotionsDelegate clCEmotions_GetOnInitializedEvent(clCEmotions emotions) NOEXCEPT;
CL_DLL clCEmotionsDelegate clCEmotions_GetOnCalibratedEvent(clCEmotions emotions) NOEXCEPT;

CLC_STRUCT(clCEmotionsDelegateString);
typedef void (*clCEmotionsHandlerString)(clCEmotions, const char*);
CL_DLL void clCEmotionsDelegateString_Set(clCEmotionsDelegateString delegate,
                                          clCEmotionsHandlerString handler) NOEXCEPT;
CL_DLL clCEmotionsDelegateString clCEmotions_GetOnErrorEvent(clCEmotions emotions) NOEXCEPT;

/**
 * \brief Initialize emotions classifier
 *
 * \param emotions classifier handler
 * \param platformAddress address of platform to upload to
 */
CL_DLL void clCEmotions_Initialize(clCEmotions emotions, const char* platformAddress) NOEXCEPT;
/**
 * \brief Start emotions calibration
 *
 * \param emotions classifier handle
 */
CL_DLL void clCEmotions_StartCalibration(clCEmotions emotions) NOEXCEPT;

#ifdef __cplusplus
}
#endif
