// mac937@nyu.edu
// hw15

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

    ReadEmployeeInfo(employee_info_pathname, employee_list);
    ReadTimesheetData(hours_worked_pathname, employee_list);
    std::vector<Employee> sorted = SortEmployeesByPay(employee_list);
    std::cout << "*********Payroll Information********" << std::endl;
    for (const Employee& employee : sorted) {
        std::cout << employee.GetName() << ", $" << employee.ComputePay() << std::endl;
    }
    std::cout << "*********End payroll**************" << std::endl;
    return 0;
}
