// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

#include "Capsule/CDefinesPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * \brief String implementation
 *
 * Object handled in Capsule.
 * Need call free by "clCString_Free(clCString stringPointer)"
 */
CLC_STRUCT(clCString);

/**
 * \brief Free memory of clCString
 */
CL_DLL void clCString_Free(clCString stringPointer) NOEXCEPT;

/**
 * \brief Get ANSI string from clCString
 *
 * \returns const char*
 */
CL_DLL const char* clCString_CStr(clCString stringPointer) NOEXCEPT;

#ifdef __cplusplus
}
#endif