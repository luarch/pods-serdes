FROM almalinux/8-base:latest AS buildenv

RUN yum install clang clang-devel cmake git -y

FROM buildenv AS release-build

COPY . /usr/src/pods-serdes

RUN cd /usr/src/pods-serdes &&\
    mkdir -p _buildc8 &&\
    cd _buildc8 &&\
    cmake -DLIBCLANG_DIR=/usr/lib64/ .. &&\
    cmake --build .

FROM scratch AS c8build-export
COPY --from=release-build /usr/src/pods-serdes/_buildc8/PodsSerdes ./PodsSerdes-C8-release
