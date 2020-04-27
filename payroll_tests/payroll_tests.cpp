#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "../q1/payroll.h"


TEST_CASE("PreciseDecimal_modulus", "") {
    PreciseDecimal p(MONEY_PRECISION);
    REQUIRE(p.modulus() == 100);
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

TEST_CASE("ReadEmployeeData", "basic") {
    using namespace std;
    istringstream emp_input(R"(9 3.50 Jonas Salk
17 6.00 Abraham P. Lincoln
2 50.99 Madonna
)");
    EmployeeList employees;
    ReadEmployeeInfo(emp_input, employees);
    vector<Employee> expected1({
           Employee(9, "Jonas Salk", PreciseDecimal::Money(3, 50)),
           Employee(17, "Abraham P. Lincoln", PreciseDecimal::Money(6, 0)),
           Employee(2, "Madonna", PreciseDecimal::Money(50, 99)),
    });
    REQUIRE(employees == expected1);
    istringstream hours_input(R"(17 40
9 10
9 20
2 1
17 40
9 10
17 5
)");
    ReadTimesheetData(hours_input, employees);
    vector<Employee> expected2(expected1.begin(), expected1.end());
    expected2[0].AddHoursWorked(10 + 20 + 10);
    expected2[1].AddHoursWorked(40 + 40 + 5);
    expected2[2].AddHoursWorked(1);
    REQUIRE(employees == expected2);
}

TEST_CASE("Employee", "ComputePay") {
    Employee emp(5, "Foo bar", PreciseDecimal::Money(5, 25));
    REQUIRE(0 == emp.ComputePay().GetNormalizedValue());
    emp.AddHoursWorked(3);
    REQUIRE(1575 == emp.ComputePay().GetNormalizedValue());
}

TEST_CASE("ReadTimesheetData", "basic") {
    using namespace std;
    vector<pair<string, vector<IntPair>>> test_cases({
             pair<string, vector<IntPair>>("", {}),
             pair<string, vector<IntPair>>("1 2", {IntPair(1, 2)}),
             pair<string, vector<IntPair>>("1 2\n", {IntPair(1, 2)}),
             pair<string, vector<IntPair>>("1 2\n3 4", {IntPair(1, 2), IntPair(3, 4)}),
             pair<string, vector<IntPair>>("1 2\n3 4\n", {IntPair(1, 2), IntPair(3, 4)}),
             pair<string, vector<IntPair>>(R"(1 2
3 4
5 8
1 16
3 32
)", {
             IntPair(1, 2),
             IntPair(3, 4),
             IntPair(5, 8),
             IntPair(1, 16),
             IntPair(3, 32),
     })});
    for (const pair<string, vector<IntPair>>& test_case : test_cases) {
        const std::string& content = test_case.first;
        istringstream in(content);
        const vector<IntPair>& expected = test_case.second;
        vector<IntPair> actual;
        ReadTimesheetData(in, actual);
        REQUIRE(expected == actual);
    }
}

TEST_CASE( "Raise", "What goes here I do not know" ) {
    REQUIRE(Raise(3, 1) == 3);
    REQUIRE(Raise(2, 2) == 4);
    REQUIRE(Raise(10, 2) == 100);
    REQUIRE(Raise(10, 0) == 1);
    REQUIRE(Raise(5, 3) == 125);
}


