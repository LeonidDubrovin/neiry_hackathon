// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CClient.h"
#include "Capsule/CDefinesPrivate.h"
#include "Capsule/CDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

CLC_CLASS_WN(LicenseManagerPrivate, clCLicenseManager);

/**
 * \brief License error
 */
typedef enum clCLicenseError {
    clC_LicenseError_OK,                             /**< No error. */
    clC_LicenseError_InvalidOrExpiredKey,            /**< License key is invalid or expired. Check your internet connection, and try again. Contact reseller manager. */
    clC_LicenseError_InvalidKeyFormatOrDeviceSerial, /**< Invalid license key format or device serial number. */
    clC_LicenseError_LicenseServiceNotFound,         /**< Cannot access license service, license service not found. */
    clC_LicenseError_LicenseServiceInaccessible,     /**< Cannot access license service, license service is inaccessible. */
    clC_LicenseError_GracePeriodExpired              /**< Unexpected license verification result. */
} clCLicenseError;

/**
 * \brief Get license manager instance.
 *
 * Only one instance of the `clCLicenseManager` class per client is allowed. The second attempt to get license manager will result in `nullptr`.
 *
 * \param client pointer to a clCClient instance
 *
 * \returns license manager instance
 */
CL_DLL clCLicenseManager clCClient_GetLicenseManager(clCClient client) NOEXCEPT;

/**
 * \brief Request license verification.
 *
 * This method requests license verification, and when it's finished, the delegate, returned by `clCLicenseManager_GetOnLicenseVerifiedEvent` is called.
 *
 * \param licenseManager pointer to a clCLicenseManager instance
 * \param licenseKey license key string
 * \param device pointer to a clCDevice instance (necessary for retrieving serial number)
 */
void clCLicenseManager_VerifyLicense(clCLicenseManager licenseManager, const char* licenseKey, clCDevice device) NOEXCEPT;
/**
 * \brief Check whether the license is valid.
 *
 * \returns true if the license is valid, otherwise false
 */
bool clCLicenseManager_IsLicenseValid(clCLicenseManager licenseManager) NOEXCEPT;

CLC_STRUCT(clCLicenseManagerDelegateLicenseVerified);
typedef void (*clCLicenseManagerHandlerLicenseVerified)(clCLicenseManager, bool, clCLicenseError);
/**
 * \brief Set callback for license verified event.
 *
 * The callback has the following parameters: `clCLicenseManager` is the delegate's owner, `bool` is a success flag
 *
 * \param delegate license verified delegate. Can be obtained using clCLicenseManager_GetOnLicenseVerifiedEvent method
 * \param callback a function which is called on event
 */
CL_DLL void
clCLicenseManagerDelegateLicenseVerified_Set(clCLicenseManagerDelegateLicenseVerified delegate,
                                             clCLicenseManagerHandlerLicenseVerified callback) NOEXCEPT;
/**
 * \brief Get delegate for license verified event.
 *
 * \param licenseManager pointer to a clCLicenseManager instance
 *
 * \returns license verified event delegate
 */
CL_DLL clCLicenseManagerDelegateLicenseVerified
clCLicenseManager_GetOnLicenseVerifiedEvent(clCLicenseManager licenseManager) NOEXCEPT;

#ifdef __cplusplus
}
#endif
