
#include <catch.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "../q1/payroll.h"


TEST_CASE("PreciseDecimal_modulus", "") {
    PreciseDecimal p(MONEY_PRECISION);
    REQUIRE(p.GetModulus() == 100);
}

TEST_CASE("PreciseDecimal_GetNormalizedValue1", "125") {
    PreciseDecimal p(MONEY_PRECISION);
    p.SetSeparatedValue(1, 25);
    REQUIRE(p.GetNormalizedValue() == 125);
}

TEST_CASE("PreciseDecimal_GetNormalizedValue2", "8") {
    PreciseDecimal p(MONEY_PRECISION);
    p.SetSeparatedValue(0, 8);
    REQUIRE(p.GetNormalizedValue() == 8);
}

TEST_CASE("PreciseDecimal_GetNormalizedValue3", "100") {
    PreciseDecimal q(MONEY_PRECISION);
    q.SetSeparatedValue(1, 0);
    REQUIRE(q.GetNormalizedValue() == 100);
}

TEST_CASE("PreciseDecimal_read", "basic") {
    using namespace std;
    vector<pair<string, PreciseDecimal>> read_test_cases({
             pair<string, PreciseDecimal>("1.50", PreciseDecimal::Money(1, 50)),
             pair<string, PreciseDecimal>("0.75", PreciseDecimal::Money(0, 75)),
             pair<string, PreciseDecimal>("0.06", PreciseDecimal::Money(0, 6)),
             pair<string, PreciseDecimal>("3.09", PreciseDecimal::Money(3, 9)),
             pair<string, PreciseDecimal>("1.50 garbage", PreciseDecimal::Money(1, 50)),
             pair<string, PreciseDecimal>("    0.75", PreciseDecimal::Money(0, 75)),
     });
    for (const pair<string, PreciseDecimal>& test_case : read_test_cases) {
        istringstream in(test_case.first);
        PreciseDecimal actual(MONEY_PRECISION);
        in >> actual;
        REQUIRE(test_case.second == actual);
    }
}

TEST_CASE("PreciseDecimal_write", "basic") {
    using namespace std;
    vector<pair<PreciseDecimal, string>> write_test_cases({
          pair<PreciseDecimal, string>(PreciseDecimal::Money(1, 50), "1.50"),
          pair<PreciseDecimal, string>(PreciseDecimal::Money(0, 8), "0.08"),
    });
    for (const pair<PreciseDecimal, string>& test_case : write_test_cases) {
        ostringstream out;
        out << test_case.first;
        if (test_case.second != out.str()) {
            cerr << "expect " << test_case.second << " == " << out.str() << endl;
        }
        REQUIRE(test_case.second == out.str());
    }
}
