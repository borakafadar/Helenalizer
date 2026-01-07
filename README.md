# Helenalizer

Helenalizer turns audio into responsive, real-time visuals you can tweak live through a GUI. It’s written in **C++17** and built with **CMake**.
It is just an audio visualizer.

It’s built around:
- **SFML** for windowing/graphics/audio I/O
- **TGUI** for the interface layer
- **OpenGL** for rendering


## What the app does (high level)

- Captures audio
- Processes audio in chunks, extracting frequency/energy information suitable for reactive visuals (e.g., spectrum-style bars)
- Renders visuals in real time while you interact with settings via GUI
- Supports asset-driven visuals (models/textures). A few small third‑party libraries are vendored in `libs/` to keep integration straightforward.

## Libraries used

### Main dependencies
- **SFML 3.0.2**
- **TGUI 1.11**
- **OpenGL**

### Vendored in `libs/`
- **miniaudio**
- **nlohmann/json** 
- **stb** 
- **tinygltf** 

## Build & run (CLion / CMake workflow)

1. Install **SFML 3.0.2** and **TGUI 1.11** on your system.
2. Open the project in **CLion**.
3. Ensure CMake config can find SFML/TGUI (you may need to adjust the library locations in the project’s CMake configuration).
4. Build and run from CLion.

## License

MIT — see [`license.txt`](license.txt).

### ps.
This stupid project was inspired by [this](https://www.instagram.com/p/DMQ9oeOudHm/).

