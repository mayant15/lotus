# Roadmap

## Wishlist
- Engine modules for rendering, resources, physics etc. These would each be separate DLLs. The game itself would be a module (see Godot)
- Engine play button for testing the game in editor
- Split simulation and rendering threads
  - Single tick or independent ticks? I would prefer to go for single tick, but then we'll have to synchronize each frame.
  - FixedUpdate vs Update for physics and rendering
- Low-level render library to interface with various graphics APIs like Vulkan, OpenGL etc. (or just use Vulkan everywhere?)
- Deferred rendering
- Shadows and reflections
- Importer tool to import an asset into the engine and store it with all required metadata in an optimized format
- Build tool to create no-editor builds of the game. Have a couple of stub files that implement the runtime and use CMake scripts
to build and link it with the engine and the game DLL
