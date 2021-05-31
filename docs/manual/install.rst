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

Command Line
+++++++++++++++++++

After cloning the repository, build with CMake::

    # In the root of the repo
    mkdir build && cd build
    conan install .. -s build_type=Debug
    cmake ..
    cmake --build .

The sets up a working directory ``build/`` for cmake and builds all targets. You should now have an output library file at
``build/lib/`` and an example executable using that file in ``build/bin/``.

Using Visual Studio
+++++++++++++++++++

For Visual Studio users, open the project as a CMake project. You should then get a few CMake errors claiming there's no ``conanbuildinfo.cmake``.
Open up a Developer Command Prompt, and perform the ``conan install`` as follows::

    cd your-cmake-binary-dir
    conan install path-to-root-of-the-lotus-repo -s build_type=Debug

Basically, we want to run ``conan install`` in your ``CMAKE_BINARY_DIR``, which will fetch all dependencies and set up CMake stubs to use them.

.. NOTE::
   Support for multiple build configurations is currently lacking, which is why Visual Studio needs this workaround and also why we passed
   the ``-s build_type=Debug`` flag to conan. The Debug build configuration is the only one that we're working with for now.

Editor Layout
+++++++++++++

You can copy the ``editor/imgui.ini`` file to the directory where the editor executable is to get the default layout. You can skip this if you want to set up your own layout.
