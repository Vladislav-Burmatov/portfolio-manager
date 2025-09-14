#ifndef ASSET_H
#define ASSET_H

#include <string>
#include <vector>

class Asset {
private:
    std::string name_;
    std::string ticker_;
    std::vector<std::string> dates_;
    std::vector<double> prices_;

public:
    Asset(const std::string& name_, const std::string& ticker_);

    std::string name();
    std::string ticker();
    std::vector<double> prices();
    std::vector<std::string> dates();

    void setPrices(std::vector<double>);

    int readPricesFromFile();
};

#endif