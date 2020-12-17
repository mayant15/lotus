FROM alpine
LABEL description="Container for building documentation for lotus"

RUN \
apk update \
&& apk add git bash python3 py3-pip cmake doxygen graphviz \
&& pip3 install sphinx sphinx_rtd_theme breathe

WORKDIR /home