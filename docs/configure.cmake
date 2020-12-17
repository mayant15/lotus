# Configure the doxyfile
# Adapted from https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/

# Find all the public headers
get_filename_component(PUBLIC_INCLUDE_DIR ../lotus/include ABSOLUTE)
file(GLOB_RECURSE LOTUS_PUBLIC_HEADERS ${PUBLIC_INCLUDE_DIR}/*.h)

# Setup directories
set(DOXYGEN_INPUT_DIR ${PUBLIC_INCLUDE_DIR}/lotus)
set(DOXYGEN_EXCLUDE_DIR ${DOXYGEN_INPUT_DIR}/internal)

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/out/doxygen)
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)

set(DOXYGEN_MAIN_PAGE ${CMAKE_CURRENT_SOURCE_DIR}/index.md)

set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${DOXYGEN_OUTPUT_DIR}/Doxyfile)

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
