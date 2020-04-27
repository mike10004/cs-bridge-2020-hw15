
#include <catch.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "../q1/payroll.h"


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

