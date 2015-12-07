# proxyd

A simple proxy for prototyping hammer-based parsing of DNP3. The main program is called "proxyd".

# building

The program uses [cmake](https://cmake.org) to generate makefiles.

1) build and install pesco's [branch of hammer](https://github.com/pesco/hammer) using scons.

In the future we might try and use cmake externals to do this, or do a cmake build for hammer. The proxy will link to the static library that this build produces (libhammer.a).

2) Clone this repository recursively to retrieve  the three git submodules: dnp3 parser, tclap (cli parser), and easy-logging.

```
> git clone --recursive <repo address>
```

3) Run cmake to generate a makefile. Recommend using a separate build directory, e.g.:

```
> mkdir build; cd build
> cmake ..
> make
```

# running

The proxy can describe its command line arguments with -h

```
> proxyd -h
USAGE: 

   ./proxyd  [-s <name>] [-n <ipv4 address>] .... other args
   
   ....
```

A full example invocation would be:

```
> ./proxyd -s dnp3 -l 20001 -a 0.0.0.0 -n 192.168.1.2 -c 20000
2015-12-07 06:21:41,390 INFO  [default] Using parser: dnp3
2015-12-07 06:21:41,407 INFO  [default] completed dnp3/hammer parser initialization
2015-12-07 06:21:41,407 INFO  [default] Listening on: 0.0.0.0:20001
```

This invocation uses the DNP3 parser, and listens on port 20001 on all adapters. It forwards connections and traffic to 192.168.1.2:20000.


