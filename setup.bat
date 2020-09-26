REM ; This script should run on a fresh clone to install all dependencies
REM ; You should not need to run this more that once.

SET VCPKG_ROOT_DIR=%CD%\external\vcpkg

REM ; Bootstrap vcpkg
%VCPKG_ROOT_DIR%\bootstrap-vcpkg.bat

REM ; Install dependencies
REM ; TODO: Detect platform
SET VCPKG_EXEC=%VCPKG_ROOT_DIR%\vcpkg.exe

%VCPKG_EXEC% install assimp:x64-windows
%VCPKG_EXEC% install glfw3:x64-windows
%VCPKG_EXEC% install pybind11:x64-windows
%VCPKG_EXEC% install spdlog:x64-windows
