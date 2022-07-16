message(STATUS "Fetching Assimp...")

FetchContent_Declare(
    assimp 
    GIT_REPOSITORY "https://github.com/assimp/assimp"
    GIT_TAG c8dafe0d2887242285c0080c6cbbea8c1f1c8094
)

set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_NO_EXPORT ON)

# Configure exporters
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)

# Configure importers
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_FBX_IMPORTER ON)
# set(ASSIMP_BUILD_BLEND_IMPORTER ON)

set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_INSTALL OFF)

FetchContent_MakeAvailable(assimp)
