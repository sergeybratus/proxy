cmake_minimum_required (VERSION 3.0)
project (proxy)
set(PROXY_MAJOR_VERSION 2)
set(PROXY_MINOR_VERSION 1)
set(PROXY_MICRO_VERSION 0)
set(PROXY_VERSION ${PROXY_MAJOR_VERSION}.${PROXY_MINOR_VERSION}.${PROXY_MICRO_VERSION})

# PkgConfig
FIND_PACKAGE(PkgConfig) # tell cmake to require pkg-config
PKG_CHECK_MODULES(GLIB2 REQUIRED glib-2.0>=2.36.0)

set(LIB_TYPE STATIC)

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_C_FLAGS "-Wall -std=c99")

# different release and debug flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# add the parser as a cmake subdirectory
# add_subdirectory(deps/dnp3)
  
# include paths for all the local libraries
include_directories(lib/src)
include_directories(lib/include)
include_directories(deps/tclap/include)
include_directories(deps/easylogging/src)
include_directories(deps/dnp3/include)

# temporary build the parser in this project until we can figure out
# how the static/dynamic linkage is inherited

# ---- parser library ----
file(GLOB_RECURSE dnp3hammer_SRC ./deps/dnp3/src/*.c)
add_library(dnp3hammer ${LIB_TYPE} ${dnp3hammer_SRC})
target_link_libraries(dnp3hammer hammer.a)

# ---- proxy library ----
file(GLOB_RECURSE proxy_SRC ./lib/src/*.cpp)
add_library(proxy ${LIB_TYPE} ${proxy_SRC})
install(TARGETS proxy DESTINATION lib)
target_link_libraries(proxy dnp3hammer)

# ---- proxyd ----
file(GLOB_RECURSE proxyd_SRC ./main/*.cpp)
add_executable(proxyd ${proxyd_SRC})
target_link_libraries(proxyd LINK_PUBLIC proxy)
