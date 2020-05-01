
#include <catch.hpp>
#include <iostream>
#include <vector>
#include <list>
#include "../q1/payroll.h"


TEST_CASE("BookkeeperTests_ReadEmployeeInfo") {
    using namespace std;
    istringstream emp_input(R"(9 3.50 Jonas Salk
17 6.00 Abraham P. Lincoln
2 50.99 Madonna
)");
    EmployeeList employees;
    Bookkeeper bookkeeper;
    bookkeeper.ReadEmployeeInfo(emp_input, employees);
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
    std::vector<TimesheetLine> timesheet_lines;
    bookkeeper.ReadTimesheetData(hours_input, timesheet_lines);
    bookkeeper.UpdateEmployees(timesheet_lines, employees);
    vector<Employee> expected2(expected1.begin(), expected1.end());
    expected2[0].AddHoursWorked(10 + 20 + 10);
    expected2[1].AddHoursWorked(40 + 40 + 5);
    expected2[2].AddHoursWorked(1);
    REQUIRE(employees == expected2);
}

TEST_CASE("Bookkeeper_ReadTimesheetData_TimesheetLine", "basic") {
    using namespace std;
    vector<pair<string, vector<TimesheetLine>>> test_cases({
             pair<string, vector<TimesheetLine>>("", {}),
             pair<string, vector<TimesheetLine>>("1 2", {TimesheetLine(1, 2)}),
             pair<string, vector<TimesheetLine>>("1 2\n", {TimesheetLine(1, 2)}),
             pair<string, vector<TimesheetLine>>("1 2\n3 4", {TimesheetLine(1, 2), TimesheetLine(3, 4)}),
             pair<string, vector<TimesheetLine>>("1 2\n3 4\n", {TimesheetLine(1, 2), TimesheetLine(3, 4)}),
             pair<string, vector<TimesheetLine>>(R"(1 2
3 4
5 8
1 16
3 32
)", {
             TimesheetLine(1, 2),
             TimesheetLine(3, 4),
             TimesheetLine(5, 8),
             TimesheetLine(1, 16),
             TimesheetLine(3, 32),
     })});
    for (const pair<string, vector<TimesheetLine>>& test_case : test_cases) {
        const std::string& content = test_case.first;
        istringstream in(content);
        const vector<TimesheetLine>& expected = test_case.second;
        vector<TimesheetLine> actual;
        Bookkeeper bookkeeper;
        bookkeeper.ReadTimesheetData(in, actual);
        REQUIRE(expected == actual);
    }
}

