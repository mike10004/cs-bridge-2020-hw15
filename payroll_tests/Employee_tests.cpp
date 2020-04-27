#include <catch.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "../q1/payroll.h"
TEST_CASE("Employee", "ComputePay") {
    Employee emp(5, "Foo bar", PreciseDecimal::Money(5, 25));
    REQUIRE(0 == emp.ComputePay().GetNormalizedValue());
    emp.AddHoursWorked(3);
    REQUIRE(1575 == emp.ComputePay().GetNormalizedValue());
}

