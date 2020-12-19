Installation
=============

Requirements
-------------

* CMake > 3.16
* A compiler toolchain with C++17 support
* A 64-bit OS
* `Conan <https://conan.io/>`_ package manager

Build
------

After cloning the repository, build with CMake::

    # In the root of the repo
    mkdir build && cd build
    conan install ..
    cmake ..
    cmake --build .

The sets up a working directory ``build/`` for cmake and builds all targets. You should now have an output library file at
``build/lib/`` and an example executable using that file in ``build/bin/``.
