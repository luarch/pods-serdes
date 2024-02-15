# PODs SerDes

The tool to help serialize and deserialize POD types with the magic of libclang.

## Build

Dependencies: clang-devel (RHEL), libclang-dev (Debian/Ubuntu)

A docker build file is provided for convenience.

```sh
./c8build.sh
```

Debug and Release binaries will then be generated under `c8build-export`.

### CMake Integration

Please see [example folder](./example) for an project example with CMake integration for both C++ and Python code generation.
