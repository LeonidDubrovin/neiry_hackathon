// Copyright. 2019 - 2024 PSBD. All rights reserved.

#pragma once

// This is a private header with API internal defines. No need to directly use/include this header to your project.

#include <Platforms.hpp>

// Linkage macro.
#if CC_PLATFORM_WIN
#ifdef CL_DYNAMIC // in case we want a dynamic library
#ifdef CL_EXPORT
#define CL_DLL __declspec(dllexport)
#else
#define CL_DLL __declspec(dllimport)
#endif
#else // if static linkage, we don't export or import anything
#define CL_DLL
#endif
#else // !CC_PLATFORM_WIN
#define CL_DLL
#endif

#define CLC_STRUCT(Name) \
    struct Name##d;      \
    typedef struct Name##d* Name

#ifdef __cplusplus
#include <cstdint>
#define NOEXCEPT noexcept

#define CLC_CLASS_WN(Wrapped, Name) \
    namespace capsule::client {     \
    class Wrapped;                  \
    }                               \
    typedef class capsule::client::Wrapped* Name

#define CLC_STRUCT_WN(Wrapped, Name) \
    namespace capsule::client {      \
    struct Wrapped;                  \
    }                                \
    typedef struct capsule::client::Wrapped const* Name

#define CLC_STRUCT_WNN(Wrapped, Name, Namespace) \
    namespace capsule::Namespace {               \
    struct Wrapped;                              \
    }                                            \
    typedef struct capsule::Namespace::Wrapped const* Name

#else
#include <stdint.h>
#define NOEXCEPT

#define CLC_CLASS_WN(Wrapped, Name) \
    struct clCWrapped##d;           \
    typedef struct clCWrapped##d* Name

#define CLC_STRUCT_WN(Wrapped, Name) \
    struct clCWrapped##d;            \
    typedef struct clCWrapped##d const* Name

#endif //__cplusplus
