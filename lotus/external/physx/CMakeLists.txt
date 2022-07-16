message(STATUS "Fetching Physx...")

FetchContent_Declare(
    physx
    GIT_REPOSITORY "https://github.com/NVIDIAGameWorks/PhysX"
    GIT_TAG c3d5537bdebd6f5cd82fcaf87474b838fe6fd5fa
)

FetchContent_MakeAvailable(physx)
