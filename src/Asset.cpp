#include "Asset.h"
#include "PriceRecord.h"
#include "Date.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

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

double Asset::getReturnBetweenDates(Date starting_date, Date ending_date, DatePolicy policy, ReturnType returns) {
    if (history_.empty() || history_.size() < 2) { throw std::runtime_error("History of " + ticker_ + " " + " is to short"); }

    double starting_price = this->getPriceOnDate(starting_date, policy);
    double ending_price = this->getPriceOnDate(ending_date, policy);

    switch (returns) {
    case ReturnType::Simple:
        return (ending_price - starting_price) / starting_price;
    case ReturnType::Logarithmic:
        return std::log(ending_price / starting_price);
    }
}

std::vector<double> Asset::getReturnArray(Date starting_date, Date ending_date, ReturnType returns) {
    if (starting_date >= this->getLastDate() || ending_date <= this->getFirstDate()) {
        throw std::runtime_error("History between " + starting_date.toString() + " and " + ending_date.toString() + " is too short");
    }

    std::vector<double> return_vector;

    auto it = std::lower_bound(history_.begin(), history_.end(), starting_date,
        [](const PriceRecord& record, const Date& date) { return record.date < date; });

    switch (returns) {
    case ReturnType::Simple:
        while (std::next(it) != history_.end()) {
            if (std::next(it)->date <= ending_date) {
                return_vector.push_back((std::next(it)->price) / (it->price) - 1);
                ++it;
            }
        }
        break;
    case ReturnType::Logarithmic:
        while (std::next(it) != history_.end()) {
            if (std::next(it)->date <= ending_date) {
                return_vector.push_back(std::log((std::next(it)->price) / (it->price)));
                ++it;
            }
        }
        break;
    }
    return return_vector;
}

std::vector<double> Asset::getReturnArray(ReturnType returns) {
    if (history_.empty() || history_.size() < 2) {
        throw std::runtime_error("History of " + ticker_ + " is too short");
    }
    
    return getReturnArray(this->getFirstDate(), this->getLastDate(), returns);
}

double Asset::getAverageReturn(Date starting_date, Date ending_date, AverageType average_type) {
    if (starting_date >= this->getLastDate() || ending_date <= this->getFirstDate()) {
        throw std::runtime_error("History between " + starting_date.toString() + " and " + ending_date.toString() + " is too short");
    }

    auto it = std::lower_bound(history_.begin(), history_.end(), starting_date,
        [](const PriceRecord& record, const Date& date) { return record.date < date; });

    double result = 0;
    double first_price = 0;
    double last_price = 0;
    int number_of_elements = 0;

    switch (average_type) {
    case AverageType::Arithmetic:
        while (std::next(it) != history_.end()) {
            if (std::next(it)->date <= ending_date) {
                result += (std::next(it)->price) / (it->price) - 1;
                number_of_elements++;
                ++it;
            }
        }
        result = result / number_of_elements;
        break;
    case AverageType::Logarithmic:
        first_price = it->price;
        while (std::next(it) != history_.end()) {
            if (std::next(it)->date <= ending_date) {
                number_of_elements++;
                ++it;
            }
        }
        last_price = it->price;
        result = std::log(last_price / first_price) / number_of_elements;
        break;
    case AverageType::Geometric:
        first_price = it->price;
        while (std::next(it) != history_.end()) {
            if (std::next(it)->date <= ending_date) {
                number_of_elements++;
                ++it;
            }
        }
        last_price = it->price;
        result = std::exp(std::log(last_price / first_price) / number_of_elements) - 1;
        break;
    }
    return result;
}

double Asset::getAverageReturn(AverageType average_type) {
    if (history_.empty()) {
        throw std::runtime_error("History of " + ticker_ + " is empty");
    }

    Date first_date = this->getFirstDate();
    Date last_date = this->getLastDate();

    return this->getAverageReturn(first_date, last_date, average_type);
}

double Asset::getVolatility(Date starting_date, Date ending_date, ReturnType returns) {
    if (history_.empty() || history_.size() < 3) {
        throw std::runtime_error("History of " + ticker_ + " is too short");
    }

    std::vector<double> return_vector = this->getReturnArray(starting_date, ending_date, returns);
    double average_return = 0;

    switch (returns) {
    case(ReturnType::Simple):
        average_return = this->getAverageReturn(starting_date, ending_date, AverageType::Arithmetic);
        break;
    case(ReturnType::Logarithmic):
        average_return = this->getAverageReturn(starting_date, ending_date, AverageType::Logarithmic);
        break;
    }

    double quadratic_deviation = 0;
    double return_at_date = 0;

    for (int i = 0; i < return_vector.size(); i++) {
        return_at_date = return_vector[i];
        quadratic_deviation += (return_at_date - average_return) * (return_at_date - average_return);
    }

    quadratic_deviation = quadratic_deviation / (return_vector.size() - 1);

    return std::sqrt(quadratic_deviation);
}

double Asset::getVolatility(ReturnType returns) {
    return this->getVolatility(this->getFirstDate(), this->getLastDate(), returns);
}