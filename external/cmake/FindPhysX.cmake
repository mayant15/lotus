# TODO: Improve this file
# Refer to #222 here https://github.com/NVIDIAGameWorks/PhysX/pull/222

# PhysX does not export its targets, like others do. Manually find files.
if (WIN32)
    set(VCPKG_LIB_PATHS_DEBUG ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/debug/lib)
    set(VCPKG_LIB_PATHS_RELEASE ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/lib)
endif(WIN32)

if (UNIX)
    set(VCPKG_LIB_PATHS_DEBUG ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-linux/debug/lib)
    set(VCPKG_LIB_PATHS_RELEASE ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-linux/lib)
endif(UNIX)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(PHYSX_LIB_DIR ${VCPKG_LIB_PATHS_RELEASE})
    message("-- Using release PhysX")
else()
    set(PHYSX_LIB_DIR ${VCPKG_LIB_PATHS_DEBUG})
    message("-- Using debug PhysX")
endif()

set(PHYSX_BASE_NAMES PhysX_64 PhysX_32 PhysX_static_64)
set(PHYSX_COMMON_NAMES PhysXCommon_64 PhysXCommon_32 PhysXCommon_static_64)
set(PHYSX_FOUNDATION_NAMES PhysXFoundation_64 PhysXFoundation_32 PhysXFoundation_static_64)
set(PHYSX_EXTENSIONS_NAMES PhysXExtensions_static_64 PhysXExtensions_static_32)
set(PHYSX_PVD_NAMES PhysXPvdSDK_static_64 PhysXPvdSDK_static_32)

find_library(PHYSX_BASE NAMES ${PHYSX_BASE_NAMES} PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_COMMON NAMES ${PHYSX_COMMON_NAMES} PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_FOUNDATION NAMES ${PHYSX_FOUNDATION_NAMES} PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_EXTENSIONS NAMES ${PHYSX_EXTENSIONS_NAMES} PATHS ${PHYSX_LIB_DIR})
find_library(PHYSX_PVD NAMES ${PHYSX_PVD_NAMES} PATHS ${PHYSX_LIB_DIR})

set(PHYSX_LINK_LIBS ${PHYSX_COMMON} ${PHYSX_FOUNDATION} ${PHYSX_BASE} ${PHYSX_EXTENSIONS} ${PHYSX_PVD})
