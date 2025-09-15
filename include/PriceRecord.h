#ifndef PRICERECORD_H
#define PRICERECORD_H

#include "Date.h"

struct PriceRecord {
    Date date;
    double price;

    PriceRecord(const Date& d, double p);
};

#endif