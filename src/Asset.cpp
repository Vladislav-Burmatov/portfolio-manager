#include "Asset.h"
#include "PriceRecord.h"
#include "Date.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <windows.h>

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

int Asset::readFromFile() {
    std::ifstream file("../../../../data/" + ticker_ + ".csv");
    std::string line;

    TCHAR buffer[MAX_PATH];
    DWORD length = GetCurrentDirectory(MAX_PATH, buffer);
    if (length != 0 && length < MAX_PATH) {
        std::wcout << L"Current working directory: " << buffer << std::endl;
    }

    if (!file.is_open()) {
        throw std::runtime_error("Couldn't read the file " + ticker_ + ".csv");
    }
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Date date;
        double price;

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
    return 0;
}