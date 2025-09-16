#include "Date.h"

#include <sstream>
#include <iomanip>
#include <vector>

Date::Date() : year(BoundedInt<1, 3000>(1)), month(BoundedInt<1, 12>(1)), day(BoundedInt<1, 31>(1)) {}

Date::Date(int m, int d, int y) : year(BoundedInt<1, 3000>(y)), month(BoundedInt<1, 12>(m)), day(BoundedInt<1, 31>(d)) {}

bool Date::operator==(const Date& other) const { return (month == other.month && day == other.day && year == other.year); }

bool Date::operator!=(const Date& other) const { return !(*this == other); }

bool Date::operator<(const Date& other) const {
    if (year < other.year) { return true; }
    else if (year > other.year) { return false; }
    if (month < other.month) { return true; }
    else if (month > other.month) { return false; }
    if (day < other.day) { return true; }
    return false;
}

bool Date::operator>(const Date& other) const { return (other < *this); }

bool Date::operator<=(const Date& other) const { return !(other < *this); }

bool Date::operator>=(const Date& other) const { return !(*this < other); }

int Date::daysBetween(const Date& other) {
    int self_days_since_0;
    int other_days_since_0;
    std::vector<int> days_since_year_start = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    self_days_since_0 = 365 * (year.value - 1) + 
                        ((year.value - 1) / 4) - 
                        ((year.value - 1) / 100) + 
                        ((year.value - 1) / 400);
    self_days_since_0 += days_since_year_start[month.value];
    if (((year.value % 4 == 0 && year.value % 100 != 0) || year.value == 400) && month.value > 2) { self_days_since_0 += 1; }
    self_days_since_0 += day.value;

    other_days_since_0 = 365 * (other.year.value - 1) +
                        ((other.year.value - 1) / 4) -
                        ((other.year.value - 1) / 100) +
                        ((other.year.value - 1) / 400);
    other_days_since_0 += days_since_year_start[month.value];
    if (((other.year.value % 4 == 0 && other.year.value % 100 != 0) || other.year.value == 400) && other.month.value > 2) { other_days_since_0 += 1; }
    other_days_since_0 += other.day.value;

    return std::abs(self_days_since_0 - other_days_since_0);
}

Date Date::fromString(const std::string& s) {
    Date date;
    char delimeter1, delimeter2;

    std::istringstream iss(s);
    if (!(iss >> date.month >> delimeter1 >> date.day >> delimeter2 >> date.year)) {
        throw std::runtime_error("Couldn't read the date from " + s);
    }
    if (delimeter1 != '/' || delimeter2 != '/') {
        throw std::runtime_error("Wrong date format, expected MM/DD/YYYY, got " + s + " instead");
    }

    return date;
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << month << "/"
        << std::setw(2) << day << "/"
        << std::setw(4) << year;
    return oss.str();
}