// mac937@nyu.edu
// hw15
//
// A note about style: This code follows the Google C++ Style Guide (mostly),
// which differs slightly from the class style guidelines, in that variable
// names are in snake_case but function names are in SentenceCase. Private
// member variables are in snake_case with a trailing underscore.
//
// This code deviates from the style guide by not defining class member
// field accessor functions with snake_case names, but instead defines
// accessor functions with names like GetX().

#include <iostream>
#include <vector>
#include <unistd.h> // stage: cut
#include <climits>
#include "payroll.h"

// stage: cut start
std::string get_working_path()  // from somewhere on stackoverflow
{
    char temp[PATH_MAX];
    return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}
// stage: cut stop

int main(int argc, char* argv[]) {
    DoublyLinkedList<Employee> employee_list;
    std::string employee_info_pathname;
    std::string hours_worked_pathname;

    std::cerr << "PWD=" << get_working_path() << std::endl; // stage: cut
    // stage: cut start
    if (argc == 3) {
        employee_info_pathname.assign(argv[1]);
        hours_worked_pathname.assign(argv[2]);
        std::cerr << "reading data from " << employee_info_pathname << " and " << hours_worked_pathname << std::endl;
    } else {
    // stage: cut stop
    std::cout << "Enter pathname of employee data file: ";
    std::cin >> employee_info_pathname;
    std::cout << "Enter pathname of timesheet data file: ";
    std::cin >> hours_worked_pathname;
    } // stage: cut

    Bookkeeper bookkeeper;
    bookkeeper.ReadEmployeeInfo(employee_info_pathname, employee_list);
    std::vector<TimesheetLine> timesheet_entries;
    bookkeeper.ReadTimesheetData(hours_worked_pathname, timesheet_entries);
    bookkeeper.UpdateEmployees(timesheet_entries, employee_list);
    std::vector<Employee> sorted = bookkeeper.SortEmployeesByPay(employee_list);
    std::cout << "*********Payroll Information********" << std::endl;
    for (const Employee& employee : sorted) {
        std::cout << employee.GetName() << ", $" << employee.ComputePay() << std::endl;
    }
    std::cout << "*********End payroll**************" << std::endl;
    return 0;
}
