#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include "../gumby/gumby.h"
#include <iostream>

TEST_CASE( "Compute the square", "[4][Square]" ) {
    std::cout << "computing the square" << std::endl;
    REQUIRE(gumby::foo(2) == 4);
    std::cout << "square computed" << std::endl;
}
