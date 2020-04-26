// mac937@nyu.edu
// hw15

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <list>
#include <algorithm>
#include <unistd.h>
#include <climits>

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

long Raise(int base, int exponent) {
    long product = 1;
    for (int i = 0; i < exponent; i++) {
        product *= base;
    }
    return product;
}

std::ostream& operator<<(std::ostream& out, const PreciseDecimal& value) {
    long modulus = value.modulus();
    long pre_point = value.normalized_value_ / modulus;
    long post_point = value.normalized_value_ % modulus;
    const char* post_prefix = post_point < 10 ? "0" : "";
    return out << pre_point << POINT << post_prefix << post_point;
}

std::istream& operator>>(std::istream& in, PreciseDecimal& value) {
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
            long factor = Raise(BASE, value.precision_ - i - 1);
            post_point_total += ch_value * factor;
        } else {
            break;
        }
    }
    value.SetSeparatedValue(pre_point, post_point_total);
    return in;
}

PreciseDecimal::PreciseDecimal(int precision) : precision_(precision), normalized_value_() {
}

void PreciseDecimal::SetNormalizedValue(long normalized_value) {
    normalized_value_ = normalized_value;
}

void PreciseDecimal::SetSeparatedValue(long pre_separator, long post_separator) {
    long modulus_ = modulus();
    long normalized_value = pre_separator * modulus_ + post_separator;
    SetNormalizedValue(normalized_value);
}

long PreciseDecimal::GetNormalizedValue() const {
    return normalized_value_;
}

long PreciseDecimal::modulus() const {
    return Raise(BASE, precision_);
}

void PreciseDecimal::Assign(const PreciseDecimal &other) {
    normalized_value_ = other.normalized_value_;
}

PreciseDecimal &PreciseDecimal::operator=(const PreciseDecimal &other) {
    Assign(other);
    return *this;
}

PreciseDecimal::PreciseDecimal(const PreciseDecimal &other)
    : precision_(other.precision_), normalized_value_(other.normalized_value_)
{
}

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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
Employee::Employee(int id, const std::string &name, const PreciseDecimal &salary_dollars_per_hour)
    : id_(id),
    name_(name),
    salary_dollars_per_hour_(salary_dollars_per_hour),
    hours_worked_(0)
{

}
#pragma clang diagnostic pop

int Employee::GetId() const {
    return id_;
}

std::string Employee::GetName() const {
    return name_;
}

PreciseDecimal Employee::ComputePay() const {
    long cents = salary_dollars_per_hour_.GetNormalizedValue() * hours_worked_;
    PreciseDecimal pay(MONEY_PRECISION);
    pay.SetNormalizedValue(cents);
    return pay;
}

void Employee::AddHoursWorked(int hours) {
    hours_worked_ += hours;
}

bool Employee::Equals(const Employee &other) const {
    return id_ == other.id_
            && name_ == other.name_
            && salary_dollars_per_hour_ == other.salary_dollars_per_hour_
            && hours_worked_ == other.hours_worked_;
}

bool Employee::operator==(const Employee &other) const {
    return Equals(other);
}

void Employee::Assign(const Employee &other) {
    id_ = other.id_;
    name_ = other.name_;
    salary_dollars_per_hour_ = other.salary_dollars_per_hour_;
    hours_worked_ = other.hours_worked_;
}

Employee& Employee::operator=(const Employee &other) {
    Assign(other);
    return *this;
}

Employee::Employee(const Employee &other) = default;

//template<class T>
//struct LinkedListNode {
//
//};

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

void ReadEmployeeInfo(std::istream& in, EmployeeList& employee_list) {
    while (in) {
        int id;
        std::string name_start;
        std::string name_remainder;
        PreciseDecimal salary(MONEY_PRECISION);
        in >> id;
        if (!in) {
            break;
        }
        in >> salary;
        if (!in) {
            break;
        }
        in >> name_start;
        if (in) {
            std::getline(in, name_remainder);
        }
        Employee employee(id, name_start + name_remainder, salary);
        employee_list.push_back(employee);
    }
}

void ReadEmployeeInfo(const std::string& pathname, EmployeeList& employee_list) {
    std::ifstream infile(pathname);
    if (infile) {
        ReadEmployeeInfo(infile, employee_list);
    } else {
        std::cerr << "error reading employee data from " << pathname << std::endl;
    }
    infile.close();
    if (employee_list.empty()) {
        std::cerr << "no employees to report on" << std::endl;
    }
}

Employee* FindEmployeeById(EmployeeList& employeeList, int id) {
    for (Employee& employee : employeeList) {
        if (employee.GetId() == id) {
            return &employee;
        }
    }
    return nullptr;
}

typedef std::pair<int, int> IntPair;

void ReadTimesheetData(std::istream& in, std::vector<IntPair>& entries) {
    while (in) {
        int employee_id;
        int hours_worked;
        in >> employee_id;
        if (in) {
            in >> hours_worked;
            entries.emplace_back(IntPair(employee_id, hours_worked));
        }
        in.ignore(INT_MAX, '\n');
    }
}

void ReadTimesheetData(std::istream& in, EmployeeList& employee_list) {
    std::vector<IntPair> entries;
    ReadTimesheetData(in, entries);
    for (const IntPair& entry : entries) {
        int employee_id = entry.first;
        int hours_worked = entry.second;
        Employee* employee = FindEmployeeById(employee_list, employee_id);
        if (employee != nullptr) {
            employee->AddHoursWorked(hours_worked);
        } else {
            std::cerr << "error: employee with id " << employee_id << " is not in the employee list" << std::endl;
        }
    }
}

void ReadTimesheetData(const std::string& pathname, EmployeeList& employee_list) {
    std::ifstream infile(pathname);
    if (infile) {
        ReadTimesheetData(infile, employee_list);
    } else {
        std::cerr << "error reading timesheet data from " << pathname << std::endl;
    }
    infile.close();
}

struct EmployeePayComparator
{
    bool operator ()(const Employee & emp1, const Employee & emp2)
    {
        // higher pay goes first
        return emp1.ComputePay().GetNormalizedValue() > emp2.ComputePay().GetNormalizedValue();
    }
};

std::vector<Employee> SortEmployeesByPay(EmployeeList& employee_list) {
    std::vector<Employee> list_copy(employee_list.begin(), employee_list.end());
    std::sort(list_copy.begin(), list_copy.end(), EmployeePayComparator());
    return list_copy;
}

std::string get_working_path()
{
    char temp[PATH_MAX];
    return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

void TestPreciseDecimal() {
    using namespace std;
    {
        PreciseDecimal p(MONEY_PRECISION);
        assert(p.modulus() == 100);
    }
    {
        PreciseDecimal p(MONEY_PRECISION);
        p.SetSeparatedValue(1, 25);
        assert(p.GetNormalizedValue() == 125);
    }
    {
        PreciseDecimal p(MONEY_PRECISION);
        p.SetSeparatedValue(0, 8);
        assert(p.GetNormalizedValue() == 8);
    }
    {
        PreciseDecimal q(MONEY_PRECISION);
        q.SetSeparatedValue(1, 0);
        assert(q.GetNormalizedValue() == 100);
    }
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
        assert(test_case.second == actual);
    }
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
        assert(test_case.second == out.str());
    }
}

void TestRaise() {
    assert(Raise(3, 1) == 3);
    assert(Raise(2, 2) == 4);
    assert(Raise(10, 2) == 100);
    assert(Raise(10, 0) == 1);
    assert(Raise(5, 3) == 125);
}

void TestReadEmployeeData() {
    using namespace std;
    istringstream emp_input(R"(9 3.50 Jonas Salk
17 6.00 Abraham P. Lincoln
2 50.99 Madonna
)");
    EmployeeList employees;
    ReadEmployeeInfo(emp_input, employees);
    list<Employee> expected1({
            Employee(9, "Jonas Salk", PreciseDecimal::Money(3, 50)),
            Employee(17, "Abraham P. Lincoln", PreciseDecimal::Money(6, 0)),
            Employee(2, "Madonna", PreciseDecimal::Money(50, 99)),
    });
    assert(employees == expected1);
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
    EmployeeList expected2l(expected2.begin(), expected2.end());
    assert(employees == expected2l);
}

void TestEmployee() {
    Employee emp(5, "Foo bar", PreciseDecimal::Money(5, 25));
    assert(0 == emp.ComputePay().GetNormalizedValue());
    emp.AddHoursWorked(3);
    assert(1575 == emp.ComputePay().GetNormalizedValue());
}

void TestReadTimesheetData() {
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
        assert(expected == actual);
    }
}

int main(int argc, char* argv[]) {
    TestRaise();
    TestPreciseDecimal();
    TestReadTimesheetData();
    TestReadEmployeeData();
    TestEmployee();
    EmployeeList employee_list;
    std::string employee_info_pathname;
    std::string hours_worked_pathname;
    std::cerr << "PWD=" << get_working_path() << std::endl; // stage: cut
    if (argc == 3) {
        employee_info_pathname.assign(argv[1]);
        hours_worked_pathname.assign(argv[2]);
        std::cerr << "reading data from " << employee_info_pathname << " and " << hours_worked_pathname << std::endl;
    } else {
        std::cout << "Enter pathname of employee data file: ";
        std::cin >> employee_info_pathname;
        std::cout << "Enter pathname of timesheet data file: ";
        std::cin >> hours_worked_pathname;
    }
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
