Installation
=============

Requirements
-------------

* CMake > 3.16
* A compiler toolchain with C++17 support
* A python interpreter on your PATH

Build
------

Clone with ``git clone --recurse-submodules`` to get all submodules. Then in the root of the repo, do::

    mkdir build && cd build
    cmake ..
    cmake --build .

.. NOTE::
   Do not clone the repository to a path with spaces. We use ``vcpkg`` for dependency management, which currently has an issue with spaces. See `this <https://github.com/microsoft/vcpkg/pull/13126>`_ pull request for more information.

You should now have an output library file at ``build/lib/`` and the example executable using that file in ``build/bin``.

Building with Visual Studio
+++++++++++++++++++++++++++

Make sure you have CMake tools installed with Visual Studio (see `this guide <https://docs.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=vs-2019>`_).
You can then just open the directory and VS will automatically set things up for you. Select `example.exe` as your startup project to build the test executable.
