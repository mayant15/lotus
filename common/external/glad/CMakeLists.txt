add_library(glad STATIC src/gl.c)
target_include_directories(glad PUBLIC include)
target_compile_definitions(glad PUBLIC GLAD_GLAPI_EXPORT PRIVATE GLAD_GLAPI_EXPORT_BUILD)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})
