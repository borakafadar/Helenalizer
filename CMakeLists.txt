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

#TGUI
set(TGUI_DIR "C:/dev/TGUI-1.11/lib/cmake/TGUI")
find_package(TGUI REQUIRED)

#OpenGL
find_package(OpenGL REQUIRED)

# Tell CMake to look in the 'src' folder for code
include_directories(src)
include_directories(libs)

# Create the executable from main.cpp
# Make the terminal go away by adding WIN32 before main.cpp
add_executable(Helenalizer WIN32 src/main.cpp
        src/AudioProcessing.cpp
        src/AudioProcessing.h
        src/VisualizerRecorder.cpp
        src/VisualizerRecorder.h
        libs/miniaudio.h
        libs/miniaudio_impl.cpp
        src/SimpleFFT.cpp
        src/SimpleFFT.h
        src/DrawObjects.cpp
        src/DrawObjects.h
        src/GUI/GUI.cpp
        src/GUI/GUI.h
        src/GUI/Button.cpp
        src/GUI/Button.h
        libs/tiny_gltf_impl.cpp
        libs/stb_impl.cpp
        libs/json.hpp
        libs/stb_image.h
        libs/stb_image_write.h
        libs/tiny_gltf.h
        src/Model3D.cpp
        src/Model3D.h
)

# Link the executable to the SFML libraries
target_link_libraries(Helenalizer
        PRIVATE
        TGUI::TGUI
        SFML::Graphics
        SFML::Audio
        SFML::Window
        SFML::System
        pthread
        opengl32
)

# Define the path to your SFML bin folder
# ADJUST THIS PATH to where you actually installed SFML
set(SFML_BIN_DIR "C:/dev/SFML-3.0.2/bin")
set(TGUI_BIN_DIR "C:/dev/TGUI-1.11/bin")

# Copy DLLs to the build directory after compilation
add_custom_command(TARGET Helenalizer POST_BUILD
        # Copy SFML DLLs
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_BIN_DIR}
        $<TARGET_FILE_DIR:Helenalizer>

        # Copy TGUI DLLs
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TGUI_BIN_DIR}
        $<TARGET_FILE_DIR:Helenalizer>

        # Copy Assets folder
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Helenalizer>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
        $<TARGET_FILE_DIR:Helenalizer>/assets

        COMMENT "Copying DLLs and assets to build directory..."
    )