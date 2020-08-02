# Lotus

An in-house game engine

## Requirements
- CMake > 3.16
- [conan](https://docs.conan.io/en/latest/getting_started.html) package manager

## Build

Clone with `git clone --recurse-submodules` to get all submodules. Then in the root of the repo, do
```shell script
mkdir build && cd build
conan install ..
cmake ..
cmake --build .
```

You should now have an output library file at `build/lib/` and the example executable using that file in `build/bin`.
There are a few absolute paths in `examples/quickstart/main.cpp`. You'll have to whatever works on your computer, till we come up with a better solution.

## Ideas
- [ ] Manage scenes with Doom-style BSP
- [ ] Multipass rendering for a better lighting solution with the scene graph
- [ ] Store scenes on disk with gLTF
- [ ] Implement python subclasses for scene nodes
- [ ] Editor GUI with [ImGui](https://github.com/ocornut/imgui) or [GTK+](https://gtk.org/)
- [ ] Setup Exhale to properly generate Sphinx documentation
- [ ] Store config in YAML using [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [ ] ECS with EnTT
- [ ] Physics systems with [bullet](https://pybullet.org/wordpress/)
- [ ] Implement the `#include` directive for GLSL
