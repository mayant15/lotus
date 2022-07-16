set(NFD_SOURCES
    src/nfd_common.c
    src/nfd_win.cpp
)

add_library(nfd STATIC ${NFD_SOURCES})
target_include_directories(nfd PUBLIC include)
