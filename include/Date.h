#ifndef DATE_H
#define DATE_H

#include "BoundedInt.h"

#include <string>

struct Date {
    BoundedInt<1, 31> day;
    BoundedInt<1, 12> month;
    BoundedInt<1, 3000> year;

    Date();
    Date(int m, int d, int y);

    bool operator==(const Date& other) const;
    bool operator!=(const Date& other) const;
    bool operator<(const Date& other) const;
    bool operator>(const Date& other) const;
    bool operator<=(const Date& other) const;
    bool operator>=(const Date& other) const;

    int daysSinceZero() const;

    int daysBetween(Date other_date);

    static Date fromString(const std::string& s);

    std::string toString() const;
};

#endif