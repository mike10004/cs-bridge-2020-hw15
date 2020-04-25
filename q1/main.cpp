// mac937@nyu.edu
// hw15

#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <list>
#include <algorithm>

const char POINT = '.';
const int MONEY_PRECISION = 100;
const int BASE = 10;

template<class T>
struct Pair {
    const T a;
    const T b;
    Pair(const T& a, const T& b);
};

class FixedPrecisionDecimal {
public:
    FixedPrecisionDecimal(int precision);
    friend std::ostream& operator<<(std::ostream& out, const FixedPrecisionDecimal& value);
    friend std::istream& operator>>(std::istream& in, FixedPrecisionDecimal& value);
    void SetNormalizedValue(long normalized_value);
    void SetSeparatedValue(long pre_separator, long post_separator);
    void SetSeparatedValue(const Pair<long>& parts);
    long GetNormalizedValue();
    Pair<long> GetSeparatedValue();
protected:
    long normalized_value_;
private:
    const int precision_;
};

long raise(int base, int exponent) {
    long product = 1;
    for (int i = 0; i < exponent; i++) {
        product *= base;
    }
    return product;
}

std::ostream& operator<<(std::ostream& out, const FixedPrecisionDecimal& value) {
    long modulus = raise(BASE, value.precision_);
    long pre_point = value.normalized_value_ / modulus;
    long post_point = value.normalized_value_ % modulus;
    return out << pre_point << POINT << post_point;
}

std::istream& operator>>(std::istream& in, FixedPrecisionDecimal& value) {
    long pre_point;
    in >> pre_point;
    char separator;
    in >> separator;
    if (separator != POINT) {
        value.SetSeparatedValue(pre_point, 0);
        return in;
    }
    long post_point_total = 0;
    for (int i = 0; i < value.precision_; i++) {
        char ch;
        in >> ch;
        if (ch >= '0' && ch <= '9') {
            int ch_value = ch - '0';
            long factor = raise(BASE, value.precision_ - i - 1);
            post_point_total += ch_value * factor;
        } else {
            break;
        }
    }
    value.SetSeparatedValue(pre_point, post_point_total);
    return in;
}

class Employee {
public:
    Employee(int id, const std::string& name, const FixedPrecisionDecimal& salary_dollars_per_hour);
    int GetId();
    const std::string& GetName();
    FixedPrecisionDecimal ComputePay();
    void AddHoursWorked(int hours);
private:
    const int id_;
    const std::string name_;
    const FixedPrecisionDecimal salary_dollars_per_hour_;
    int hours_worked_;
};

template<class T>
struct LinkedListNode {

};

//template<class T>
//class DoublyLinkedList {
//public:
//    DoublyLinkedList();
//    virtual ~DoublyLinkedList();
//    void push_front(const T& item);
//    void push_back(const T& item);
//    T pop_front();
//    T pop_bak();
//    bool empty();
//    size_t size();
//private:
//    LinkedListNode<T>* head_;
//    LinkedListNode<T>* tail_;
//    size_t count_;
//};

typedef std::list<Employee> EmployeeList;

void ReadEmployeeInfo(const std::istream& in, EmployeeList& employee_list) {

}

void ReadEmployeeInfo(const std::string& pathname, EmployeeList& employee_list) {
    std::ifstream infile(pathname);
    if (infile) {
        ReadEmployeeInfo(infile, employee_list);
    } else {
        std::cerr << "error reading employee data from " << pathname << std::endl;
    }
}

Employee* FindEmployee(EmployeeList& employeeList, int id) {
    for (Employee& employee : employeeList) {
        if (employee.GetId() == id) {
            return &employee;
        }
    }
    return nullptr;
}

void ReadTimesheetData(const std::istream& in, EmployeeList& employee_list) {

}

void ReadTimesheetData(const std::string& pathname, EmployeeList& employee_list) {
    std::ifstream infile(pathname);
    if (infile) {
        ReadTimesheetData(infile, employee_list);
    } else {
        std::cerr << "error reading timesheet data from " << pathname << std::endl;
    }
}

void SortEmployeesByPay(EmployeeList& employee_list) {

}

template<class F, class T>
class Function
{
public:
    Function();
    virtual ~Function() = default;
    virtual T Apply(const F& item) const = 0;
};

template<class T>
void ApplyToAll(EmployeeList& employee_list, const Function<Employee, T>& action) {
    for (const Employee& employee : employee_list) {
        action.Apply(employee);
    }
}

class PrintFunction : public Function<Employee, void>
{
public:
    PrintFunction();
    void Apply(const Employee& employee) const override;
};

int main(int argc, char* argv[]) {
    EmployeeList employee_list;
    std::string employee_info_pathname;
    std::string hours_worked_pathname;
    if (argc == 3) {
        employee_info_pathname.assign(argv[1]);
        hours_worked_pathname.assign(argv[2]);
    } else {
        std::cout << "Enter pathname of employee data file: ";
        std::cin >> employee_info_pathname;
        std::cout << "Enter pathname of timesheet data file: ";
        std::cin >> hours_worked_pathname;
    }
    ReadEmployeeInfo(employee_info_pathname, employee_list);
    ReadTimesheetData(hours_worked_pathname, employee_list);
    SortEmployeesByPay(employee_list);
    ApplyToAll(employee_list, PrintFunction());
    return 0;
}
