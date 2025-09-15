#ifndef ASSET_H
#define ASSET_H

#include "PriceRecord.h"

#include <string>
#include <vector>

class Asset {
private:
    std::string name_;
    std::string ticker_;
    std::vector<PriceRecord> quotes_;

public:
    Asset(const std::string& name_, const std::string& ticker_);

    std::string name();
    std::string ticker();
    std::vector<double> prices();
    std::vector<PriceRecord> quotes();

    void readFromFile();

    double getYields(Date starting, Date ending);
};

#endif