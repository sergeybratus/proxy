cmake_minimum_required (VERSION 3.0)
project (proxy)
set(PROXY_MAJOR_VERSION 2)
set(PROXY_MINOR_VERSION 1)
set(PROXY_MICRO_VERSION 0)
set(PROXY_VERSION ${PROXY_MAJOR_VERSION}.${PROXY_MINOR_VERSION}.${PROXY_MICRO_VERSION})

# other options off-by-default that you can enable
option(WERROR "Set all warnings to errors" OFF)

if(STATICLIBS)
      set(LIB_TYPE STATIC)
else()
      set(LIB_TYPE SHARED)
endif()

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")

# different release and debug flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
  
# include paths for all the local libraries
include_directories(./libs/src)
include_directories(./libs/include)

# ---- proxy library ----
file(GLOB_RECURSE proxy_SRC ./libs/src/proxy/*.cpp)
add_library(proxy ${LIB_TYPE} ${proxy_SRC})
install(TARGETS proxy DESTINATION lib)

# ---- main ----
add_executable(main ./main/main.cpp)
target_link_libraries(main LINK_PUBLIC proxy)

