message(STATUS "Fetching Bullet...")

FetchContent_Declare(
    bullet
    GIT_REPOSITORY "https://github.com/mayant15/bullet3"
    GIT_TAG 0fa4df9d3d7ea008aeaaaa48539bf4c2dcb75f69
)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_EXAMPLE OFF)
set(BUILD_ENET OFF)
set(BUILD_CLSOCKET OFF)
set(USE_DOUBLE_PRECISION ON)

FetchContent_MakeAvailable(bullet)
