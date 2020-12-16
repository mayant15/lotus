FROM gcc:9.3.0
LABEL description="Container for testing the lotus game engine"

WORKDIR /usr/src

# System level dependencies
RUN apt update && apt -y upgrade
RUN apt-get install -y cmake python3-pip doxygen ninja-build xorg-dev libx11-xcb-dev libxcb-render0-dev \
libxcb-render-util0-dev libxcb-xkb-dev libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev \
libxcb-shape0-dev libxcb-sync-dev libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-dri3-dev libxcb-util0-dev \
libgl1-mesa-dev graphviz

# pip dependencies
RUN pip3 install sphinx sphinx_rtd_theme breathe conan

# Configure conan
RUN conan profile new default --detect
RUN conan profile update settings.compiler.libcxx=libstdc++11 default
