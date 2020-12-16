# Dockerfile for studiocentauri/conan:gcc-9.3.0

FROM conanio/gcc9:latest
LABEL description="Container for testing the lotus game engine"

# System level dependencies
RUN \
sudo apt update \
&& sudo apt install -y xorg-dev libx11-xcb-dev libxcb-render0-dev libxcb-render-util0-dev libxcb-xkb-dev \
libxcb-icccm4-dev libxcb-image0-dev libxcb-keysyms1-dev libxcb-randr0-dev libxcb-shape0-dev libxcb-sync-dev \
libxcb-xfixes0-dev libxcb-xinerama0-dev xkb-data libxcb-dri3-dev libxcb-util0-dev libgl1-mesa-dev
