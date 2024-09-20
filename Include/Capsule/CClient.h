// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CError.h"
#include "Capsule/CString.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Reason for client's disconnect
 */
typedef enum clCDisconnectReason {
    clC_DR_UserRequested,
    clC_DR_Destruction,
    clC_DR_FatalError
} clCDisconnectReason;

/**
 * \brief Connection state to Capsule.
 */
typedef enum clCConnectionState {
    clC_CS_Disconnected,
    clC_CS_Connecting,
    clC_CS_Connected,
    clC_CS_Disconnecting
} clCConnectionState;

/**
 * \brief Used to connect with Capsule.
 *
 * Handle to client used to connect with Capsule, responsible for control.
 */
CLC_CLASS_WN(ClientPrivate, clCClient);

/**
 * \brief Client callback without parameters.
 * (Note: ALL callbacks receive their owner as first argument).
 */
CLC_STRUCT(clCClientDelegate);
typedef void (*clCClientHandler)(clCClient);
CL_DLL void clCClientDelegate_Set(clCClientDelegate delegate,
                                  clCClientHandler callback) NOEXCEPT;
/**
 * \brief Client disconnect callback. Not supported in the current version
 */
CLC_STRUCT(clCClientDelegateDisconnectReason);
typedef void (*clCClientHandlerDisconnectReason)(clCClient,
                                                 clCDisconnectReason);
CL_DLL void clCClientDelegateDisconnectReason_Set(
    clCClientDelegateDisconnectReason delegate,
    clCClientHandlerDisconnectReason callback) NOEXCEPT;
/**
 * \brief Client error callback. Not supported in the current version
 */
CLC_STRUCT(clCClientDelegateError);
typedef void (*clCClientHandlerError)(clCClient, clCError);
CL_DLL void clCClientDelegateError_Set(clCClientDelegateError delegate,
                                       clCClientHandlerError callback) NOEXCEPT;

/**
 * Get Capsule client version
 *
 * \returns client version string
 */
CL_DLL clCString clCClient_GetVersionString() NOEXCEPT;

/**
 * \brief Client application version setter. Must be set before client connection
 * \param[in,out] client client handle
 * \param[in] version string, representing client's version
 */
CL_DLL void clCClient_SetAppVersion(clCClient client, const char* version);

/**
 * Create a Capsule client.
 *
 * \returns client handle
 */
CL_DLL clCClient clCClient_Create() NOEXCEPT;
/**
 * Create a Capsule client with application name.
 * \param clientName application name
 *
 * \returns client handle
 */
CL_DLL clCClient clCClient_CreateWithName(const char* clientName) NOEXCEPT;
/**
 * Create a Capsule client with data directory.
 * \param dataDir path to data directory
 *
 * \returns client handle
 */
CL_DLL clCClient clCClient_CreateWithDataDirectory(const char* dataDir) NOEXCEPT;
/**
 * Create a Capsule client with data directory and application name.
 * \param dataDir path to data directory
 * \param clientName application name
 *
 * \returns client handle
 */
CL_DLL clCClient clCClient_CreateWithDataDirectoryAndName(const char* dataDir,
                                                          const char* clientName) NOEXCEPT;
/**
 * Destroy Capsule client. Release the handle.
 *
 * \param client client handle
 */
CL_DLL void clCClient_Destroy(clCClient client) NOEXCEPT;
/**
 * Connect to Capsule.
 *
 * \param client client handle
 * \param address Capsule address to connect to. If NULL, the default address
 * "tcp:://127.0.0.1:5666" is in effect.
 * * For embedded Capsule use in-process protocol: "inproc://capsule",
 * * Otherwise use tcp protocol: "tcp:://127.0.0.1:5666".
 * OnConnected event is called when connected.
 */
CL_DLL void clCClient_Connect(clCClient client, const char* address) NOEXCEPT;
/**
 * Connect to Capsule with access token.
 *
 * \param client client handle
 * \param address Capsule address to connect to. If NULL, the default address
 * "tcp:://127.0.0.1:5666" is in effect.
 * * For embedded Capsule use in-process protocol: "inproc://capsule",
 * * Otherwise use tcp protocol: "tcp:://127.0.0.1:5666".
 * OnConnected event is called when connected.
 * \param accessToken access token for communication with platform
 */
CL_DLL void clCClient_ConnectWithAccessToken(clCClient client,
                                             const char* address,
                                             const char* accessToken) NOEXCEPT;
/**
 * Connect to Capsule with access token and platform address.
 *
 * \param client client handle
 * \param address Capsule address to connect to. If NULL, the default address
 * "tcp:://127.0.0.1:5666" is in effect.
 * * For embedded Capsule use in-process protocol: "inproc://capsule",
 * * Otherwise use tcp protocol: "tcp:://127.0.0.1:5666".
 * OnConnected event is called when connected.
 * \param accessToken access token for communication with platform
 * \param platformAddress URL of a platform processing http requests
 */
CL_DLL void clCClient_ConnectWithAccessTokenAndPlatformAddress(clCClient client,
                                                               const char* address,
                                                               const char* accessToken,
                                                               const char* platformAddress) NOEXCEPT;

/**
 * Get delegate for client RefreshAccessTokenRequested event.
 *
 * \param client client handle
 * \returns client delegate
 */
CL_DLL clCClientDelegate
clCClient_GetOnRefreshAccessTokenRequestedEvent(clCClient client) NOEXCEPT;
/**
 * Refresh access token for data upload
 * \param client client handle
 * \param accessToken access token
 */
CL_DLL void clCClient_RefreshToken(clCClient client, const char* accessToken) NOEXCEPT;
/**
 * Get Capsule time for synchronization with EEG timestamps
 * \return time
 */
CL_DLL uint64_t clCClient_GetTime() NOEXCEPT;
/**
 * Get Capsule time in microseconds
 * \return time in microseconds
 */
CL_DLL uint64_t clCClient_GetTimeMicro() NOEXCEPT;
/**
 * Disconnects from Capsule, stopping any background threads.
 *
 * \param client client handle
 */
CL_DLL void clCClient_Disconnect(clCClient client) NOEXCEPT;
/**
 * Is client connected.
 *
 * \param client client handle
 */
CL_DLL bool clCClient_IsConnected(clCClient client) NOEXCEPT;
/**
 * Is client connecting.
 *
 * \param client client handle
 */
CL_DLL bool clCClient_IsConnecting(clCClient client) NOEXCEPT;

/**
 * Get Capsule version string.
 *
 * \param client client handle
 *
 * \returns capsule version as a string ("unknown" if not connected).
 */
CL_DLL clCString clCClient_GetCapsuleVersion(clCClient client) NOEXCEPT;
/**
 * Get Client application name.
 *
 * \param client client handle
 *
 * \returns client name as a string ("unknown" if not connected).
 */
CL_DLL clCString clCClient_GetClientName(clCClient client) NOEXCEPT;
/**
 * Get Capsule data directory.
 *
 * \param client client handle
 *
 * \returns capsule data directory as a string ("" if not applicable).
 */
CL_DLL clCString clCClient_GetDataDirectory(clCClient client) NOEXCEPT;
/**
 * Client is single-threaded.
 * \b Note: Update does all the work and must be called
 * regularly to process events.
 *
 * \param client client handle
 */
CL_DLL void clCClient_Update(clCClient client) NOEXCEPT;

/**
 * Get delegate for client connected event.
 *
 * \param client client handle
 * \returns client delegate
 */
CL_DLL clCClientDelegate clCClient_GetOnConnectedEvent(clCClient client) NOEXCEPT;
/**
 * Get delegate for client disconnected event.
 *
 * \param client client handle
 * \returns client disconnect delegate
 */
CL_DLL clCClientDelegateDisconnectReason
clCClient_GetOnDisconnectedEvent(clCClient client) NOEXCEPT;
/**
 * Get delegate for client error event.
 *
 * \param client client handle
 * \returns client error delegate
 */
CL_DLL clCClientDelegateError clCClient_GetOnErrorEvent(clCClient client) NOEXCEPT;

#ifdef __cplusplus
}
#endif
