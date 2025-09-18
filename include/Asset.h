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

    enum class AverageType { Arithmetic, Geometric, Logarithmic };

    int findDateInHistory(Date target_date);

    double getPriceOnDate(Date date, DatePolicy policy);

    std::vector<double> getReturnArray(Date starting_date, Date ending_date, ReturnType returns);

    std::vector<double> getReturnArray(ReturnType returns);

    double getReturnBetweenDates(Date starting_date, Date ending_date, DatePolicy policy, ReturnType returns);

    double getAverageReturn(Date starting_date, Date ending_date, AverageType average_type);

    double getAverageReturn(AverageType average_type);
};

#endif