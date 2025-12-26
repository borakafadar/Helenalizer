cmake_minimum_required(VERSION 3.10)

# Project name
project(Helenalizer)

# Use C++ 17 standard (modern features)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the SFML package (Graphics, Window, Audio, and System modules)
# Note: You must have SFML installed on your computer for this to work.
set(SFML_DIR "C:/dev/SFML-3.0.2/lib/cmake/SFML")
find_package(SFML 3.0.2 COMPONENTS Graphics Audio Window System REQUIRED)

# Tell CMake to look in the 'src' folder for code
include_directories(src)

# Create the executable from main.cpp
add_executable(Helenalizer src/main.cpp
        src/AudioProcessing.cpp
        src/AudioProcessing.h
        src/VisualizerRecorder.cpp
        src/VisualizerRecorder.h
        libs/miniaudio.h
        libs/miniaudio_impl.cpp
        src/SimpleFFT.cpp
        src/SimpleFFT.h
        src/DrawObjects.cpp
        src/DrawObjects.h)

# Link the executable to the SFML libraries
target_link_libraries(Helenalizer
        PRIVATE
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
)

# Define the path to your SFML bin folder
# ADJUST THIS PATH to where you actually installed SFML
set(SFML_BIN_DIR "C:/dev/SFML-3.0.2/bin")

# Copy DLLs to the build directory after compilation
add_custom_command(TARGET Helenalizer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_BIN_DIR}
        $<TARGET_FILE_DIR:Helenalizer>
        COMMENT "Copying SFML DLLs to build directory..."
)

# Copy the "assets" folder from your Source Code -> Build Folder
file(COPY ${CMAKE_SOURCE_DIR}/src/assets DESTINATION ${CMAKE_BINARY_DIR})