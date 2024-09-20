// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CClient.h"
#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CDevice.h"
#include "Capsule/CEEGTimedData.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Session state.
 */
typedef enum clCSessionState {
    clC_SS_Unknown,
    clC_SS_Starting,
    clC_SS_Active,
    clC_SS_Inactive,
    clC_SS_Pausing,
    clC_SS_Paused,
    clC_SS_Stopping
} clCSessionState;

/**
 * \brief Session error.
 */
typedef enum clCSessionError {
    clC_SE_Unknown,
    clC_SE_FailedToStart,
    clC_SE_FailedToStop,
    clC_SE_LostConnection
} clCSessionError;

/**
 * \brief Client session.
 *
 * Required for classifiers and calibrators
 */
CLC_CLASS_WN(SessionPrivate, clCSession);

/**
 * \brief Session error callback. Not supported in the current version
 */
CLC_STRUCT(clCSessionDelegateSessionError);
typedef void (*clCSessionHandlerSessionError)(clCSession, clCSessionError);
CL_DLL void
clCSessionDelegateSessionError_Set(clCSessionDelegateSessionError delegate,
                                   clCSessionHandlerSessionError callback) NOEXCEPT;

/**
 * \brief Create a Capsule session. Requires a device.
 *
 * \param client client handle
 * \param device device handle
 * \returns session handle
 */
CL_DLL clCSession clCClient_CreateSessionWithError(clCClient client, clCDevice device,
                                                   clCError* error) NOEXCEPT;
CL_DLL clCSession clCClient_CreateSessionWithMonopolarChannelsWithError(clCClient client,
                                                                        clCDevice device, clCError* error) NOEXCEPT;

typedef enum clCUserActivity {
    clCUserActivity1,
    clCUserActivity2,
    clCUserActivity3,
    clCUserActivity4,
    clCUserActivity5,
    clCUserActivityNone
} clCUserActivity;
/**
 * \brief Destroy Capsule session. Release the handle.
 *
 * \param session session handle
 */
CL_DLL void clCSession_Destroy(clCSession session) NOEXCEPT;
/**
 * \brief Get numeric ID of the session.
 *
 * \param session session handle
 * \returns int64 ID of the session
 */
CL_DLL int64_t clCSession_GetSessionID(clCSession session) NOEXCEPT;
/**
 * \brief Get UUID of the session.
 *
 * \param session session handle
 * \returns string UUID of the session
 */
CL_DLL clCString clCSession_GetSessionUUID(clCSession session) NOEXCEPT;
/**
 * \brief Get session state.
 *
 * \param session session handle
 */
CL_DLL clCSessionState clCSession_GetSessionState(clCSession session) NOEXCEPT;
/**
 * \brief Start the session.
 *
 * \param session session handle
 */
CL_DLL void clCSession_Start(clCSession session) NOEXCEPT;
/**
 * \brief Stop the session.
 *
 * \param session session handle
 */
CL_DLL void clCSession_Stop(clCSession session) NOEXCEPT;
/**
 * \brief Is the session active.
 *
 * \param session session handle
 */
CL_DLL bool clCSession_IsActive(clCSession session) NOEXCEPT;

/**
 * \brief Set extra data to send to platform
 *
 * \param session session handle
 * \param json extra data in json format
 */
CL_DLL void clCSession_SetExtra(clCSession session, const char* json) NOEXCEPT;

/**
 * \brief Mark user activity start/stop
 *
 * \param session
 * \param marker user activity marker or None for the end of user activity
 */
CL_DLL void clCSession_MarkActivity(clCSession session, clCUserActivity marker) NOEXCEPT;
/**
 * \brief Check if EEG in the session is filtered
 *
 * \param session
 * \return \c true if EEG is filtered
 */
CL_DLL bool clCSession_IsFilterEEG(clCSession session) NOEXCEPT;
/**
 * \brief Check if bipolar mode is enabled for the session
 *
 * \param session
 * \return \c true if bipolar mode is enabled
 */
CL_DLL bool clCSession_IsBipolarMode(clCSession session) NOEXCEPT;

/**
 * Session delegate without parameters. Not supported in the current version
 */
CLC_STRUCT(clCSessionDelegate);
typedef void (*clCSessionHandler)(clCSession);
CL_DLL void clCSessionDelegate_Set(clCSessionDelegate delegate,
                                   clCSessionHandler callback) NOEXCEPT;

/**
 * \brief Get session delegate for started event.
 *
 * \param session session handle
 */
CL_DLL clCSessionDelegate
clCSession_GetOnSessionStartedEvent(clCSession session) NOEXCEPT;
/**
 * \brief Get session delegate for stopped event.
 *
 * \param session session handle
 */
CL_DLL clCSessionDelegate
clCSession_GetOnSessionStoppedEvent(clCSession session) NOEXCEPT;

/**
 * \brief Get session delegate for error event.
 *
 * \param session session handle
 */
CL_DLL clCSessionDelegateSessionError
clCSession_GetOnErrorEvent(clCSession session) NOEXCEPT;

/**
 * \brief Session delegate for EEG data event. Session EEG data may be filtered and in bipolar mode depending on session configuration.
 */
CLC_STRUCT(clCSessionDelegateSessionEEGData);
typedef void (*clCSessionHandlerSessionEEGData)(clCSession, clCEEGTimedData);
CL_DLL void
clCSessionDelegateSessionEEGData_Set(clCSessionDelegateSessionEEGData delegate,
                                     clCSessionHandlerSessionEEGData callback) NOEXCEPT;

/**
 * \brief Get delegate for session EEG data event. Session EEG data may be filtered and in bipolar mode depending on session configuration.
 *
 * \param session session handle
 */
CL_DLL clCSessionDelegateSessionEEGData
clCSession_GetOnSessionEEGDataEvent(clCSession device) NOEXCEPT;

/**
 * \deprecated Use clCClient_CreateSessionWithMonopolarChannels to create session in monopolar mode
 *
 * \brief Configure whether channels in the session should work in bipolar mode
 *
 * \param session
 * \param bipolarMode \c true if bipolar mode is enabled
 * \param filterEEG \c true if raw EEG should be filtered
 */
[[deprecated("Use clCClient_CreateSessionWithMonopolarChannels to create session in monopolar mode")]] CL_DLL void clCSession_SetConfig(clCSession session, bool bipolarMode, bool filterEEG) NOEXCEPT;

#ifdef __cplusplus
}
#endif
