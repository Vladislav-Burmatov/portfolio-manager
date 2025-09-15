#ifndef DATE_H
#define DATE_H

#include <string>

struct Date {
    int day;
    int month;
    int year;

    Date();
    Date(int m, int d, int y);

    bool Date::operator==(const Date& other) const;
    bool Date::operator!=(const Date& other) const;
    bool Date::operator<(const Date& other) const;
    bool Date::operator>(const Date& other) const;
    bool Date::operator<=(const Date& other) const;
    bool Date::operator>=(const Date& other) const;

    static Date fromString(const std::string& s);

    std::string toString() const;
};

#endif