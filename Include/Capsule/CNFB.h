// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CError.h"
#include "Capsule/CNFBCalibrator.h"
#include "Capsule/CSession.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief NFB call result.
 */
typedef enum clCNFBCallResult {
    clC_NFB_Success,                      /**< Call has finished successfully. */
    clC_NFB_NotInitialized,               /**< NFB interface has not been initialized. */
    clC_NFB_ModelIsNotTrained_Deprecated, /**< Model was not trained, must be trained
                                  first. */
    clC_NFB_ModelIsTrained_Deprecated,    /**< Model is already trained, does not need any
                                  training. */
    clC_NFB_FailedToSendData              /**< Failed to send data, session might not be
                                             active. */
} clCNFBCallResult;

/**
 * \brief NFB state.
 */
typedef enum ccCNFBState {
    cc_NFB_Undefined,
    cc_NFB_Relaxation,
    cc_NFB_Concentration
} ccCNFBState;

/**
 * \brief User state, determined by NFB classifier.
 */
typedef struct clCNFBUserState {
    /**
     * \brief User's neural feedback.
     */
    const float* feedbackData;
    uint64_t feedbackSize;
    int64_t timestamp;
} clCNFBUserState;

/**
 * \brief Interface to NFB classification process.
 *
 * Before calling any of the functions make sure that:
 * * A valid \ref clCSession created with clCClient_CreateSession is passed to
 * the constructor.
 * * \ref clCSession is active.
 *
 * In order to train model you should:
 * * Mark each state at least once, with duration no less than a time window
 */
CLC_CLASS_WN(ClassificationNFBPrivate, clCNFB);
CLC_STRUCT(clCNFBDelegateNFBUserState);
typedef void (*clCNFBHandlerNFBUserState)(clCNFB, const clCNFBUserState*);
CL_DLL void clCNFBDelegateNFBUserState_Set(clCNFBDelegateNFBUserState delegate,
                                           clCNFBHandlerNFBUserState callback) NOEXCEPT;
/**
 * Create a NFB classifier.
 *
 * \param session session handle. Valid session must be passed (does not need to be active)
 * \returns NFB handle.
 */
CL_DLL clCNFB clCNFB_Create(clCSession session) NOEXCEPT;
/**
 * Destroy NFB classifier. Release the handle.
 *
 * \param nfb NFB handle
 */
CL_DLL void clCNFB_Destroy(clCNFB nfb) NOEXCEPT;

CLC_STRUCT(clCNFBDelegate);
typedef void (*clCNFBHandler)(clCNFB);
CL_DLL void clCNFBDelegate_Set(clCNFBDelegate delegate, clCNFBHandler callback) NOEXCEPT;

CLC_STRUCT(clCNFBDelegateBool);
typedef void (*clCNFBHandlerBool)(clCNFB, bool);
CL_DLL void clCNFBDelegateBool_Set(clCNFBDelegateBool delegate,
                                   clCNFBHandlerBool callback) NOEXCEPT;

CLC_STRUCT(clCNFBDelegateString);
typedef void (*clCNFBHandlerString)(clCNFB, const char*);
CL_DLL void clCNFBDelegateString_Set(clCNFBDelegateString delegate,
                                     clCNFBHandlerString callback) NOEXCEPT;

CL_DLL clCNFBDelegate clCNFB_GetOnInitializedEvent(clCNFB nfb) NOEXCEPT;
CL_DLL clCNFBDelegateNFBUserState clCNFB_GetOnUserStateChangedEvent(clCNFB nfb) NOEXCEPT;
CL_DLL clCNFBDelegateString clCNFB_GetOnErrorEvent(clCNFB nfb) NOEXCEPT;

/**
 * \brief Request initialization.
 *
 * OnInitialized event is called on completion.
 *
 * \param nfb NFB handle
 *
 * \returns call result.
 */
CL_DLL clCNFBCallResult clCNFB_Initialize(clCNFB nfb) NOEXCEPT;
/**
 * \brief Request initialization.
 *
 * OnInitialized event is called on completion.
 *
 * \param nfb NFB handle
 * \param userStateCheckPeriod period of user state updates
 *
 * \returns call result.
 */
CL_DLL clCNFBCallResult clCNFB_InitializeWithPeriod(clCNFB nfb, float userStatePeriod) NOEXCEPT;
/**
 * \brief Add calculation of neuro feedback with configured output.
 *
 * \param nfb NFB handle
 * \param feedbackFunction function of bands, e.g. "(alpha + beta) / theta"
 *
 * \returns call result.
 */
CL_DLL clCNFBCallResult clCNFB_AddFeedbackFunction(clCNFB nfb, const char* feedbackFunction) NOEXCEPT;

/**
 * \brief Check whether interface was initialized.
 *
 * \param nfb NFB handle
 *
 * \returns whether interface is initialized.
 */
CL_DLL bool clCNFB_IsInitialized(clCNFB nfb) NOEXCEPT;

/**
 * Create a NFB classifier after individual NFB has calibrated.
 *
 * \param calibrator NFB calibrator handle. A calibrated calibrator must be passed.
 * \returns NFB handle.
 */
CL_DLL clCNFB clCNFB_CreateCalibrated(clCNFBCalibrator calibrator, clCError* error) NOEXCEPT;

#ifdef __cplusplus
}
#endif
