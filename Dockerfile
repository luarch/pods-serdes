FROM almalinux/8-base:latest AS buildenv
RUN yum install clang clang-devel cmake git -y

FROM buildenv AS copysrc
COPY . /usr/src/pods-serdes

FROM copysrc AS debug-build
RUN cd /usr/src/pods-serdes &&\
    mkdir -p _buildc8 &&\
    cd _buildc8 &&\
    cmake -DLIBCLANG_DIR=/usr/lib64/ -DCMAKE_BUILD_TYPE=debug .. &&\
    cmake --build .

FROM copysrc AS release-build
RUN cd /usr/src/pods-serdes &&\
    mkdir -p _buildc8 &&\
    cd _buildc8 &&\
    cmake -DLIBCLANG_DIR=/usr/lib64/ -DCMAKE_BUILD_TYPE=release .. &&\
    cmake --build .

FROM scratch AS c8build-export
COPY --from=debug-build /usr/src/pods-serdes/_buildc8/PodsSerdes ./PodsSerdes-C8-debug
COPY --from=release-build /usr/src/pods-serdes/_buildc8/PodsSerdes ./PodsSerdes-C8-release
