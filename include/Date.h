#ifndef DATE_H
#define DATE_H

#include <string>

struct Date {
    int day;
    int month;
    int year;

    Date();
    Date(int m, int d, int y);

    static Date fromString(const std::string& s);

    std::string toString();
};

#endif