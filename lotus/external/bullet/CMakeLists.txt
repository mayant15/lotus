message(STATUS "Fetching Bullet...")

FetchContent_Declare(
    bullet
    GIT_REPOSITORY "https://github.com/bulletphysics/bullet3"
    GIT_TAG 7dee3436e747958e7088dfdcea0e4ae031ce619e
)

set(BUILD_SHARED_LIBS OFF)

set(BUILD_CPU_DEMOS OFF)
set(BUILD_OPENGL3_DEMOS OFF)
set(BUILD_BULLET2_DEMOS OFF)

set(BUILD_PYBULLET OFF)
set(BUILD_ENET OFF)
set(BUILD_CLSOCKET OFF)
set(BUILD_UNIT_TESTS OFF)

FetchContent_MakeAvailable(bullet)
