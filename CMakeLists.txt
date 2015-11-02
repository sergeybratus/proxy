cmake_minimum_required (VERSION 3.0)
project (proxy)
set(PROXY_MAJOR_VERSION 2)
set(PROXY_MINOR_VERSION 1)
set(PROXY_MICRO_VERSION 0)
set(PROXY_VERSION ${PROXY_MAJOR_VERSION}.${PROXY_MINOR_VERSION}.${PROXY_MICRO_VERSION})

# other options off-by-default that you can enable
option(WERROR "Set all warnings to errors" OFF)

# PkgConfig
FIND_PACKAGE(PkgConfig) # tell cmake to require pkg-config
PKG_CHECK_MODULES(GLIB2 REQUIRED glib-2.0>=2.36.0)

if(STATICLIBS)
      set(LIB_TYPE STATIC)
else()
      set(LIB_TYPE SHARED)
endif()

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_C_FLAGS "-Wall -std=c99")

# different release and debug flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
  
# include paths for all the local libraries
include_directories(libs/src)
include_directories(libs/include)
include_directories(deps/tclap/include)
include_directories(deps/easylogging/src)
include_directories(deps/dnp3/include)
include_directories(deps/dnp3)
# for the dnp3-hammer tests
include_directories(${GLIB2_INCLUDE_DIRS})
# for the C++ integration tests of the proxy
include_directories(./deps/catch)

# ---- dnp3 filter ----
file(GLOB_RECURSE dnp3hammer_SRC ./deps/dnp3/src/*.c ./deps/dnp3/dissect.c ./deps/dnp3/filter.c)
add_library(dnp3hammer ${LIB_TYPE} ${dnp3hammer_SRC})
target_link_libraries(dnp3hammer hammer)

# ---- test suite ----
add_executable(dnp3-tests ./deps/dnp3/test.c)
target_link_libraries(dnp3-tests dnp3hammer ${GLIB2_LIBRARIES})

# ---- plugin library ----
file(GLOB_RECURSE plugin_SRC libs/src/plugin/*.cpp)
add_library(plugin ${LIB_TYPE} ${plugin_SRC})

# ---- dnp3cpp adapter ----
file(GLOB_RECURSE dnp3cpp_SRC libs/src/dnp3cpp/*.cpp)
add_library(dnp3cpp ${LIB_TYPE} ${dnp3cpp_SRC})
target_link_libraries(dnp3cpp plugin dnp3hammer)

# ---- dnp3 plugin tests ----
file(GLOB_RECURSE dnp3plugintests_SRC ./test/dnp3/*.cpp)
add_executable(dnp3-plugin-tests ${dnp3plugintests_SRC})
target_link_libraries(dnp3-plugin-tests dnp3cpp)

# ---- proxy library ----
file(GLOB_RECURSE proxy_SRC ./libs/src/proxy/*.cpp)
add_library(proxy ${LIB_TYPE} ${proxy_SRC})
install(TARGETS proxy DESTINATION lib)
target_link_libraries(proxy plugin)

# ---- proxyd ----
file(GLOB_RECURSE proxyd_SRC ./main/*.cpp)
add_executable(proxyd ${proxyd_SRC})
target_link_libraries(proxyd LINK_PUBLIC proxy dnp3cpp)

# ---- dnp3-app-afl
file(GLOB_RECURSE dnp3_app_afl_SRC ./afl/main.cpp)
add_executable(dnp3-app-afl ${dnp3_app_afl_SRC})
target_link_libraries(dnp3-app-afl LINK_PUBLIC dnp3hammer)