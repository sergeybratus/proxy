
#include <catch.hpp>

#include <dnp3cpp/DNP3Factory.h>

using namespace proxy::dnp3;

#define SUITE(name) "Plugin - " name

TEST_CASE(SUITE("Construct and delete"))
{
    DNP3Factory factory;
}
