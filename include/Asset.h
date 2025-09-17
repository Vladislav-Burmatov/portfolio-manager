#ifndef ASSET_H
#define ASSET_H

#include "PriceRecord.h"
#include "Date.h"

#include <string>
#include <vector>

class Asset {
private:
    std::string name_;
    std::string ticker_;
    std::vector<PriceRecord> history_;

public:
    Asset(const std::string& name_, const std::string& ticker_);

    std::string name();
    std::string ticker();
    std::vector<double> prices();
    std::vector<Date> dates();
    std::vector<PriceRecord> history();

    size_t getHistoryLength();

    Date getFirstDate();
    Date getLastDate();

    void loadFromFile();

    enum class DatePolicy { Exact, Closest, PreviousAvailable, NextAvailable };

    enum class ReturnType { Simple, Logarithmic };

    double getPriceOnDate(Date date, DatePolicy policy = DatePolicy::Closest);

    double getReturnBetweenDates(Date starting, Date ending, DatePolicy policy = DatePolicy::Closest, ReturnType returns = ReturnType::Simple);

    std::vector<double> getAllReturns(ReturnType returns = ReturnType::Simple);
};

#endif