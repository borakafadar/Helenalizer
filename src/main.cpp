#include <windows.h> // For MessageBox
#include <iostream>
#include <exception>
#include "GUI/GUI.h"

int main() {
    try {
        // Run the game
        GUI gui;
        gui.run();
    }
    catch (const std::exception& e) {
        // If it crashes, SHOW A POPUP BOX
        std::string errorMsg = "Application Error: \n";
        errorMsg += e.what();
        MessageBoxA(NULL, errorMsg.c_str(), "Helenalizer Crashed", MB_ICONERROR | MB_OK);
        return -1;
    }
    catch (...) {
        MessageBoxA(NULL, "Unknown Error occurred!", "Helenalizer Crashed", MB_ICONERROR | MB_OK);
        return -1;
    }

    return 0;
}