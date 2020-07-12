# Lotus

An in-house game engine

## Requirements
- CMake > 3.16
- [conan](https://docs.conan.io/en/latest/getting_started.html) package manager

## Build

```shell script
mkdir build && cd build
conan install ..
cmake ..
cmake --build .
```

You should now have an output library file at `build/lib/` and the example executable using that file in `build/bin`.
There are a few absolute paths in `examples/quickstart/main.cpp`. You'll have to whatever works on your computer, till we come up with a better solution.

## Ideas
[x] GUI with [ImGui](https://github.com/ocornut/imgui) or [GTK+](https://gtk.org/)
[] Store config in YAML using [yaml-cpp](https://github.com/jbeder/yaml-cpp)
[] Implement ECS. References: [EntityX](https://github.com/alecthomas/entityx), [EntityPlus](https://github.com/Yelnats321/EntityPlus)
[] Better image handling with [graphicsmagick](http://www.graphicsmagick.org/)
[] Use a wrapper over a logging library. [glog](https://github.com/google/glog) or [spdlog](https://github.com/gabime/spdlog)
[] Physics systems. [React](https://www.reactphysics3d.com/), [bullet](https://pybullet.org/wordpress/)
