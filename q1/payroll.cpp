#include "payroll.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

long Raise(int base, int exponent) {
    long product = 1;
    for (int i = 0; i < exponent; i++) {
        product *= base;
    }
    return product;
}

std::ostream& operator<<(std::ostream& out, const PreciseDecimal& value) {
    long modulus = value.GetModulus();
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
    long modulus_ = GetModulus();
    long normalized_value = pre_separator * modulus_ + post_separator;
    SetNormalizedValue(normalized_value);
}

long PreciseDecimal::GetNormalizedValue() const {
    return normalized_value_;
}

long PreciseDecimal::GetModulus() const {
    return Raise(BASE, precision_);
}

bool PreciseDecimal::Equals(const PreciseDecimal &other) const {
    return precision_ == other.precision_ && normalized_value_ == other.normalized_value_;
}

bool PreciseDecimal::operator==(const PreciseDecimal &other) const {
    return Equals(other);
}

// stage: cut start
PreciseDecimal PreciseDecimal::Money(int dollars, int cents) {
    PreciseDecimal p(MONEY_PRECISION);
    p.SetSeparatedValue(dollars, cents);
    return p;
}
// stage: cut stop

#pragma clang diagnostic push                               // stage: cut
#pragma ide diagnostic ignored "modernize-pass-by-value"    // stage: cut
Employee::Employee(int id, const std::string &name, const PreciseDecimal &salary_dollars_per_hour)
        : id_(id),
          name_(name),
          salary_dollars_per_hour_(salary_dollars_per_hour),
          hours_worked_(0)
{

}
#pragma clang diagnostic pop                                // stage: cut

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

bool Employee::operator!=(const Employee &other) const {
    return !(*this == other);
}

#pragma clang diagnostic push   // stage: cut
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"  // stage: cut
void Bookkeeper::ReadEmployeeInfo(std::istream& in, EmployeeList& employee_list) {
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
        employee_list.PushBack(employee);
    }
}

void Bookkeeper::ReadEmployeeInfo(const std::string& pathname, EmployeeList& employee_list) {
    std::ifstream infile(pathname);
    if (infile) {
        ReadEmployeeInfo(infile, employee_list);
    } else {
        std::cerr << "error reading employee data from " << pathname << std::endl;
    }
    infile.close();
    if (employee_list.IsEmpty()) {
        std::cerr << "no employees to report on" << std::endl;
    }
}


class EmployeeIdPredicate : public Predicate<Employee> {
public:
    explicit EmployeeIdPredicate(int id) :id_(id) {
    }
    bool Evaluate(const Employee& employee) const override {
        return id_ == employee.GetId();
    }
private:
    int id_;
};

Employee* Bookkeeper::FindEmployeeById(EmployeeList& employeeList, int id) {
    EmployeeIdPredicate predicate(id);
    return employeeList.FindElement(predicate);
}

void Bookkeeper::ReadTimesheetData(std::istream& in, std::vector<TimesheetLine>& entries) {
    while (in) {
        int employee_id;
        int hours_worked;
        in >> employee_id;
        if (in) {
            in >> hours_worked;
            entries.emplace_back(employee_id, hours_worked);
        }
        in.ignore(INT_MAX, '\n');
    }
}

void Bookkeeper::UpdateEmployees(const std::vector<TimesheetLine> &entries, EmployeeList &employee_list) {
    for (const TimesheetLine& entry : entries) {
        Employee* employee = FindEmployeeById(employee_list, entry.employee_id);
        if (employee != nullptr) {
            employee->AddHoursWorked(entry.hours_worked);
        } else {
            std::cerr << "error: employee with id " << entry.employee_id << " is not in the employee list" << std::endl;
        }
    }
}

void Bookkeeper::ReadTimesheetData(const std::string& pathname, std::vector<TimesheetLine>& entries) {
    std::ifstream infile(pathname);
    if (infile) {
        ReadTimesheetData(infile, entries);
    } else {
        std::cerr << "error reading timesheet data from " << pathname << std::endl;
    }
    infile.close();
}
struct EmployeePayComparator
{
    bool operator ()(const Employee & emp1, const Employee & emp2) {
        // higher pay goes first
        return emp1.ComputePay().GetNormalizedValue() > emp2.ComputePay().GetNormalizedValue();
    }
};

std::vector<Employee> Bookkeeper::SortEmployeesByPay(EmployeeList& employee_list) {
    EmployeeList list_copy(employee_list);
    std::vector<Employee> employee_vector;
    while (!list_copy.IsEmpty()) {
        Employee employee = list_copy.GetFront();
        list_copy.PopFront();
        employee_vector.push_back(employee);
    }
    std::sort(employee_vector.begin(), employee_vector.end(), EmployeePayComparator());
    return employee_vector;
}

#pragma clang diagnostic pop   // stage: cut

#pragma clang diagnostic push // stage: cut
#pragma ide diagnostic ignored "modernize-use-equals-default" // stage: cut
Bookkeeper::Bookkeeper() {
}
#pragma clang diagnostic pop // stage: cut

TimesheetLine::TimesheetLine(int employee_id, int hours_worked)
    : employee_id(employee_id), hours_worked(hours_worked)
{
}

bool TimesheetLine::operator==(const TimesheetLine &other) const {
    return employee_id == other.employee_id && hours_worked == other.hours_worked;
}

