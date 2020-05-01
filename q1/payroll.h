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

/**
 * Computes the value of a base raised to an exponent.
 * @param base base
 * @param exponent exponent
 * @return value of base raised to the exponent
 */
long Raise(int base, int exponent);

/**
 * Class that represents a precise value.
 * This helps us read and print money amounts to two decimal places.
 */
class PreciseDecimal {
public:
    /**
     * Constructs an instance.
     * @param precision number of decimal places to print
     */
    explicit PreciseDecimal(int precision);
    friend std::ostream& operator<<(std::ostream& out, const PreciseDecimal& value);
    friend std::istream& operator>>(std::istream& in, PreciseDecimal& value);
    /**
     * Sets the normalized value.
     * @param normalized_value  value
     */
    void SetNormalizedValue(long normalized_value);
    void SetSeparatedValue(long pre_separator, long post_separator);
    /**
     * Gets the normalized value. The normalized value for
     * money is the total number of cents that in an amount;
     * for example, $1.50 is 150 cents, so 150 is the normalized value.
     * @return normalized value
     */
    long GetNormalizedValue() const;
    long GetModulus() const;
    bool Equals(const PreciseDecimal& other) const;
    bool operator==(const PreciseDecimal& other) const;
    static PreciseDecimal Money(int dollars, int cents);   // stage: cut
protected:
    long normalized_value_;
private:
    int precision_;
};

/**
 * Class that represents an employee.
 */
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
private:
    int id_;
    std::string name_;
    PreciseDecimal salary_dollars_per_hour_;
    int hours_worked_;
};

typedef DoublyLinkedList<Employee> EmployeeList;

/**
 * Struct that represents a timesheet entry.
 */
struct TimesheetLine {
    int employee_id;
    int hours_worked;
    TimesheetLine(int employee_id, int hours_worked);
    bool operator==(const TimesheetLine& other) const;
};

/**
 * Class that represents a bookkeeper.
 */
class Bookkeeper {
public:
    Bookkeeper();

    /**
     * Reads an employee list from a stream. Each line is expected
     * to be in the format {@code 17 $5.00 Foo Bar} where 17 is the
     * employee ID, $5.00 is the hourly wage, and Foo Bar is the employee name.
     * @param in  stream
     * @param employee_list  list to populate
     */
    void ReadEmployeeInfo(std::istream& in, EmployeeList& employee_list);

    /**
     * Reads employee info from a file.
     * @param pathname pathname of file
     * @param employee_list list to populate
     */
    void ReadEmployeeInfo(const std::string& pathname, EmployeeList& employee_list);

    /**
     * Reads timesheet entries from a file.
     * @param pathname pathname of file
     * @param entries list to populate
     */
    void ReadTimesheetData(const std::string& pathname, std::vector<TimesheetLine>& entries);

    /**
     * Reads timesheet entries from a stream.
     * Each line is expected to be in the format {@code 17 40}
     * where 17 is the employee id and 40 is the hours worked.
     * @param in stream
     * @param entries  list to populate
     */
    void ReadTimesheetData(std::istream& in, std::vector<TimesheetLine>& entries);

    /**
     * Updates the hours worked by each employee based on a set of timesheet lines.
     * @param entries timesheet lines
     * @param employee_list employees to be updated
     */
    void UpdateEmployees(const std::vector<TimesheetLine>& entries, EmployeeList& employee_list);

    /**
     * Copies an employee list to a vector and sorts it in descending order by pay total.
     * @param employee_list employees list
     * @return sorted vector of employees
     */
    std::vector<Employee> SortEmployeesByPay(EmployeeList& employee_list);
private:
    Employee* FindEmployeeById(EmployeeList& employeeList, int id);
};

#endif //HW15_PAYROLL_H
