#include "Asset.h"
#include "PriceRecord.h"
#include "Date.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Asset::Asset(const std::string& name, const std::string& ticker) : name_(name), ticker_(ticker) {}

std::string Asset::name() { return name_; }

std::string Asset::ticker() { return ticker_; }

std::vector<PriceRecord> Asset::quotes() { return quotes_; }

std::vector<double> Asset::prices() { 
    std::vector<double> prices;
    for (int i = 0; i < quotes_.size(); i++) {
        prices.push_back(quotes_[i].price);
    }
    return prices; 
}

void Asset::readFromFile() {
    std::ifstream file("../../../../data/" + ticker_ + ".csv");
    std::string line;

    if (!file.is_open()) {
        throw std::runtime_error("Couldn't read the file " + ticker_ + ".csv");
    }
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Date date;
        double price=1;

        int token_index = 0;
        while (std::getline(ss, token, ',')) {
            if (token_index == 0) {
                date = Date::fromString(token);
            }
            else if (token_index == 1) {
                price = std::stof(token.substr(1));
            }
            token_index++;
        }
        quotes_.push_back(PriceRecord(date, price));
    }

    file.close();

    std::reverse(quotes_.begin(), quotes_.end());
}

double Asset::getYields(Date starting_date, Date ending_date) {
    int i = 0;
    double starting_price = 1;
    double ending_price = 1;
    while (i < quotes_.size()) {
        if (quotes_[i].date == starting_date) {
            starting_price = quotes_[i].price;
        }
        if (quotes_[i].date == ending_date) {
            ending_price = quotes_[i].price;
        }
        i++;
    }
    return ending_price / starting_price - 1;
}