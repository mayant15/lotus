Installation
=============

Requirements
-------------

* CMake > 3.16
* A compiler with C++17 support
* [conan](https://docs.conan.io/en/latest/getting_started.html) package manager

**NOTE:** We're in the process of removing conan as a package manager. All dependencies will eventually be added as submodules.
But for now, you'll need conan.

Build
------

Clone with ``git clone --recurse-submodules`` to get all dependencies. Then in the root of the repo, do::

   mkdir build && cd build
   conan install ..
   cmake ..
   cmake --build .

You should now have an output library file at `build/lib/` and the example executable using that file in `build/bin`.
There are a few absolute paths in `examples/quickstart/main.cpp`. You'll have to change them to whatever works on your computer, till we come up with a better solution.
