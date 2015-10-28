# Instructions

Build and install hammer using AFL instrumentation

```
> sudo CC=afl-gcc scons install prefix=/usr
```

Build and install the proxy project using cmake: (assuming build dir is a subdirectory of project root)

```
> CC=afl-gcc CXX=afl-g++ cmake ..\
```
