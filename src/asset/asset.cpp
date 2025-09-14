#include "asset.h"

#include <fstream>
#include <sstream>
#include <iostream>

Asset::Asset(const std::string& name, const std::string& ticker)
    : name_(name), ticker_(ticker) {}

std::string Asset::name() { return name_; }

std::string Asset::ticker() { return ticker_; }

std::vector<double> Asset::prices() { return prices_; }

std::vector<std::string> Asset::dates() { return dates_; }

void Asset::setPrices(std::vector<double> data) {
    prices_ = data;
}

int Asset::readPricesFromFile() {
    std::ifstream file("../../../data/" + ticker_ + ".csv");
    std::string line;

    if (!file.is_open()) {
        std::cout << "Couldn't find the data file " << ticker_ << ".csv";
    }
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;

        int token_index = 0;
        while (std::getline(ss, token, ',')) {
            if (token_index == 0) {
                dates_.push_back(token);
            }
            else if (token_index == 1) {
                prices_.push_back(std::stof(token.substr(1)));
            }
            token_index++;
        }
    }
    file.close();
    return 0;
}