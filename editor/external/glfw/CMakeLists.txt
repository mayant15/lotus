message(STATUS "Fetching GLFW...")

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    GIT_TAG 45ce5ddd197d5c58f50fdd3296a5131c894e5527
)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(glfw)
