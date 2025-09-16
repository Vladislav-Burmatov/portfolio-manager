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

std::vector<PriceRecord> Asset::history() { return history_; }

void Asset::loadFromFile() {
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
        double price = 1;

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
        history_.push_back(PriceRecord(date, price));
    }

    file.close();

    std::reverse(history_.begin(), history_.end());
}

std::vector<double> Asset::prices() { 
    std::vector<double> prices;
    for (int i = 0; i < history_.size(); i++) {
        prices.push_back(history_[i].price);
    }
    return prices; 
}

std::vector<Date> Asset::dates() {
    std::vector<Date> prices;
    for (int i = 0; i < history_.size(); i++) {
        prices.push_back(history_[i].date);
    }
    return prices;
}

size_t Asset::getHistoryLength() { return history_.size(); }

Date Asset::getFirstDate() { return history_[0].date; }

Date Asset::getLastDate() { return history_[history_.size() - 1].date; }

double Asset::getPriceOnDate(Date target_date, DatePolicy policy) {
    if (history_.empty()) { 
        throw std::runtime_error("Empty history for " + ticker_ + " " + name_);
    }

    auto it = std::lower_bound(history_.begin(), history_.end(), target_date,
        [](const PriceRecord& record, const Date& date) { return record.date < date; });

    switch (policy) {
        case DatePolicy::Exact:
            if (it != history_.end() && it->date == target_date) { return it->price; }
            throw std::runtime_error("No data on " + target_date.toString() + " for " + ticker_ + " " + name_);

        case DatePolicy::PreviousAvailable:
            if (it != history_.begin()) {
                if (it != history_.end() && it->date == target_date) { return it->price; }
                --it;
                return it->price;
            }
            throw std::runtime_error("No records earlier than " + target_date.toString() + " for " + ticker_ + " " + name_);

        case DatePolicy::NextAvailable:
            if (it != history_.end()) { return it->price; }
            throw std::runtime_error("No records later than " + target_date.toString() + " for " + ticker_ + " " + name_);

        case DatePolicy::Closest:
            if (it == history_.begin()) { return it->price; }
            if (it == history_.end()) { return std::prev(it)->price; }

            int days_between_prev = target_date.daysBetween(std::prev(it)->date);
            int days_between_next = target_date.daysBetween(it->date);
            if (days_between_next < days_between_prev) { return it->price; }
            return std::prev(it)->price;
    }
}

double Asset::getYields(Date starting_date, Date ending_date) {
    int i = 0;
    double starting_price = 1;
    double ending_price = 1;
    while (i < history_.size()) {
        if (history_[i].date == starting_date) {
            starting_price = history_[i].price;
        }
        if (history_[i].date == ending_date) {
            ending_price = history_[i].price;
        }
        i++;
    }
    return ending_price / starting_price - 1;
}