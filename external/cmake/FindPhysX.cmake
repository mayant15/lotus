# TODO: Improve this file
# Refer to #222 here https://github.com/NVIDIAGameWorks/PhysX/pull/222

include(FindPackageHandleStandardArgs)

find_library(
    PHYSX_LIBRARY
    NAMES PhysX_64 PhysX_32
    PATHS
        ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/bin
        ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/lib
)

set(PHYSX_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/vcpkg/installed/x64-windows/include)

find_package_handle_standard_args(
    PhysX
    DEFAULT_MSG
    PHYSX_LIBRARY
    PHYSX_INCLUDE_DIR
)
