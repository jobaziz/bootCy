

# ============================================================================
# =========================  PROJECT SETUP  ==================================
# ============================================================================

cmake_minimum_required(VERSION 3.23)

# set the project name and languages
project(image_builder)
project(image_builder VERSION 3.0.0)
project(image_builder LANGUAGES C)

# Create a common target for compile definitions
add_library(common INTERFACE)

# enable debug/release configs
set(CMAKE_DEBUG_POSTFIX d)

# Include cargs, a library for command-line parsing
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/cargs) 

# Include crypto, our Crypto library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/crypto)

# add the executable
add_executable(image_builder
        main.c
        src/cli.c
        src/header.c
        src/body.c
        src/footer.c
        src/utils.c
)
set_target_properties(image_builder PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
# =============================================================================



# =============================================================================
# =========================  PROJECT CONFIG  ==================================
# =============================================================================

# let the preprocessor know about the system name
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
target_compile_definitions(common INTERFACE IS_LINUX) 
endif()
if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
target_compile_definitions(common INTERFACE IS_MACOS) # No Macros for MacOS as of yet.
endif()
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
target_compile_definitions(common INTERFACE IS_WINDOWS) 
endif()

set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure a header file to pass some of the CMake settings
# to the source code
set(APP_VERSION_MAJOR 3)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_PATCH 0)

# set header version info
set(HEADER_VERSION_MAJOR 1)
set(HEADER_VERSION_MINOR 1)
set(HEADER_VERSION_PATCH 0)

# built time
string( TIMESTAMP currentTime "%Y%m%dT%H%M" )
set(TIMESTAMP ${currentTime})

include_directories(${PROJECT_BINARY_DIR}/config)
configure_file(${PROJECT_SOURCE_DIR}/config/ImageBuilderConfig.h.in ${PROJECT_BINARY_DIR}/config/ImageBuilderConfig.h @ONLY)

# =============================================================================


# =============================================================================
# =========================  PROJECT LINKING  =================================
# =============================================================================

# add the binary tree to the search path for include files
# so that we will find AppImageBuilderConfig.h
target_include_directories(image_builder PUBLIC
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
    ${PROJECT_BINARY_DIR}/config
)

target_link_libraries(image_builder PRIVATE common)
target_link_libraries(image_builder PUBLIC cargs)
target_link_libraries(image_builder PUBLIC cyclone_crypto)
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
  target_link_libraries(image_builder PUBLIC pthread) # Needed on Linux to compile crypto
endif()

# =============================================================================


