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

long Raise(int base, int exponent);

template<class T>
struct LinkedListNode {
    LinkedListNode<T>* next;
    LinkedListNode<T>* prev;
    T content;
};

template<class T>
class DoublyLinkedList {
public:
    DoublyLinkedList();
    virtual ~DoublyLinkedList();
    void push_front(const T& item);
    void push_back(const T& item);
    T pop_front();
    T pop_back();
    bool empty();
    size_t size();
    std::vector<T> ToVector();
private:
    LinkedListNode<T>* head_;
    LinkedListNode<T>* tail_;
    size_t count_;
};

const char POINT = '.';
const int MONEY_PRECISION = 2;
const int BASE = 10;

class PreciseDecimal {
public:
    explicit PreciseDecimal(int precision);
    PreciseDecimal(const PreciseDecimal& other);
    friend std::ostream& operator<<(std::ostream& out, const PreciseDecimal& value);
    friend std::istream& operator>>(std::istream& in, PreciseDecimal& value);
    void SetNormalizedValue(long normalized_value);
    void SetSeparatedValue(long pre_separator, long post_separator);
    long GetNormalizedValue() const;
    long modulus() const;
    void Assign(const PreciseDecimal& other);
    PreciseDecimal& operator=(const PreciseDecimal& other);
    bool Equals(const PreciseDecimal& other) const {
        return precision_ == other.precision_ && normalized_value_ == other.normalized_value_;
    }
    bool operator==(const PreciseDecimal& other) const {
        return Equals(other);
    }
    static PreciseDecimal Money(int dollars, int cents) {
        PreciseDecimal p(MONEY_PRECISION);
        p.SetSeparatedValue(dollars, cents);
        return p;
    }
protected:
    long normalized_value_;
private:
    const int precision_;
};

class Employee {
public:
    Employee(int id, const std::string& name, const PreciseDecimal& salary_dollars_per_hour);
    Employee(const Employee& other);
    int GetId() const;
    std::string GetName() const;
    PreciseDecimal ComputePay() const;
    void AddHoursWorked(int hours);
    bool Equals(const Employee& other) const;
    bool operator==(const Employee& other) const;
    void Assign(const Employee& other);
    Employee& operator=(const Employee& other);
private:
    int id_;
    std::string name_;
    PreciseDecimal salary_dollars_per_hour_;
    int hours_worked_;
};

typedef std::list<Employee> EmployeeList;

void ReadEmployeeInfo(std::istream& in, EmployeeList& employee_list);
void ReadEmployeeInfo(const std::string& pathname, EmployeeList& employee_list);
Employee* FindEmployeeById(EmployeeList& employeeList, int id);
typedef std::pair<int, int> IntPair;

void ReadTimesheetData(std::istream& in, std::vector<IntPair>& entries);
void ReadTimesheetData(std::istream& in, EmployeeList& employee_list);
void ReadTimesheetData(const std::string& pathname, EmployeeList& employee_list);

struct EmployeePayComparator
{
    bool operator ()(const Employee & emp1, const Employee & emp2);
};

std::vector<Employee> SortEmployeesByPay(EmployeeList& employee_list);

#endif //HW15_PAYROLL_H
