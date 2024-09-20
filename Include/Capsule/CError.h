// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

/**
 * \brief Capsule error
 */
typedef enum clCError {
    clC_Error_OK = 0,
    clC_Error_FailedToConnect,
    clC_Error_FailedToInitConnection,
    clC_Error_FailedToInitialize,
    clC_Error_DeviceError,
    clC_Error_IndividualNFBNotCalibrated,
    clC_Error_NotReceived,
    clC_Error_UnlicensedAccess,
    clC_Error_UNKNOWN = 99
} clCError;
