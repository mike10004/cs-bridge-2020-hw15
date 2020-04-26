#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "../gumby/gumby.h"
#include <iostream>

using namespace gumby;

TEST_CASE( "Raise", "What goes here I do not know" ) {
    REQUIRE(Raise(3, 1) == 3);
    REQUIRE(Raise(2, 2) == 4);
    REQUIRE(Raise(10, 2) == 100);
    REQUIRE(Raise(10, 0) == 1);
    REQUIRE(Raise(5, 3) == 125);
}
