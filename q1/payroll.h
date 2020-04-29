#ifndef HW15_PAYROLL_H
#define HW15_PAYROLL_H

#include <cstddef>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <list>
#include <algorithm>
#include <unistd.h>
#include <climits>
#include "linkedlist.h"

const char POINT = '.';
const int MONEY_PRECISION = 2;
const int BASE = 10;

long Raise(int base, int exponent);

class PreciseDecimal {
public:
    explicit PreciseDecimal(int precision);
    PreciseDecimal(const PreciseDecimal& other);
    friend std::ostream& operator<<(std::ostream& out, const PreciseDecimal& value);
    friend std::istream& operator>>(std::istream& in, PreciseDecimal& value);
    void SetNormalizedValue(long normalized_value);
    void SetSeparatedValue(long pre_separator, long post_separator);
    long GetNormalizedValue() const;
    long GetModulus() const;
    void Assign(const PreciseDecimal& other);
    PreciseDecimal& operator=(const PreciseDecimal& other);
    bool Equals(const PreciseDecimal& other) const;
    bool operator==(const PreciseDecimal& other) const;
    static PreciseDecimal Money(int dollars, int cents);
protected:
    long normalized_value_;
private:
    const int precision_;
};

class Employee {
public:
    Employee(int id, const std::string& name, const PreciseDecimal& salary_dollars_per_hour);
    int GetId() const;
    std::string GetName() const;
    PreciseDecimal ComputePay() const;
    void AddHoursWorked(int hours);
    bool Equals(const Employee& other) const;
    bool operator==(const Employee& other) const;
    bool operator!=(const Employee& other) const;
    void Assign(const Employee& other);
    Employee& operator=(const Employee& other);
private:
    int id_;
    std::string name_;
    PreciseDecimal salary_dollars_per_hour_;
    int hours_worked_;
};

typedef DoublyLinkedList<Employee> EmployeeList;

void ReadEmployeeInfo(std::istream& in, EmployeeList& employee_list);
void ReadEmployeeInfo(const std::string& pathname, EmployeeList& employee_list);

struct TimesheetLine {
    int employee_id;
    int hours_worked;
    TimesheetLine(int employee_id, int hours_worked);
    bool operator==(const TimesheetLine& other) const;
};

Employee* FindEmployeeById(EmployeeList& employeeList, int id);
void ReadTimesheetData(std::istream& in, std::vector<TimesheetLine>& entries);
void ReadTimesheetData(std::istream& in, EmployeeList& employee_list);
void ReadTimesheetData(const std::string& pathname, EmployeeList& employee_list);

std::vector<Employee> SortEmployeesByPay(EmployeeList& employee_list);

#endif //HW15_PAYROLL_H
