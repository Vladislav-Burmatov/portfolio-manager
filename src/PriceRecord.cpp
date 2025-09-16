#include "PriceRecord.h"

PriceRecord::PriceRecord(const Date& d, double p) : date(d), price(p) {}

bool PriceRecord::operator==(const PriceRecord& other) const { return (date == other.date && price == other.price); }
bool PriceRecord::operator!=(const PriceRecord& other) const { return !(*this == other); }