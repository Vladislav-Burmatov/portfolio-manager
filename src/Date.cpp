#include "Date.h"

#include <sstream>
#include <iomanip>

Date::Date() : year(0), month(0), day(0) {}

Date::Date(int m, int d, int y) : year(y), month(m), day(d) {}

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

std::string Date::toString() {
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << month << "/"
        << std::setw(2) << day << "/"
        << std::setw(4) << year;
    return oss.str();
}