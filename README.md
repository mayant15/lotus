# Lotus

An in-house game engine

## Requirements
- CMake > 3.16
- A compiler toolchain that supports C++17

## Build

Clone with `git clone --recurse-submodules` to get all submodules. Then in the root of the repo, do
```shell script
mkdir build && cd build
cmake ..
cmake --build .
```

You should now have an output library file at `build/lib/` and the example executable using that file in `build/bin`.

### Building with Visual Studio

Make sure you have CMake tools installed with Visual Studio (see [this guide](https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019)).
You can then just open the directory and VS will automatically set things up for you. Select `example.exe` as your startup project to build the test executable.

## Ideas
- [ ] JSON serialization
- [ ] Filesystem solution (Something like Godot's `res://`?)
- [ ] Render lighting and continue on with learnopengl
- [ ] Multipass rendering for a better lighting solution with the scene graph
- [ ] Implement python subclasses for scene nodes
- [ ] Editor GUI with [ImGui](https://github.com/ocornut/imgui)
- [ ] Physics systems with PhysX
- [ ] Implement the `#include` directive for GLSL
- [ ] Deal with uniforms in a better way
