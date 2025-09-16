#include <iostream>
#include <string>

#include "Asset.h"

int main() {

	Date d(8, 29, 2025);

	std::string name = "Apple regular stock";
	std::string ticker = "AAPL";
	Asset A(name, ticker);
	A.loadFromFile();

	std::cout << ticker << " " << name << "\n";
	std::cout << A.getFirstDate().toString() << " " << A.getLastDate().toString() << "\n";
	std::cout << A.getHistoryLength() << " quotes \n";
	std::cout << "closest " << A.getPriceOnDate(d, Asset::DatePolicy::Closest) << "\n"
		<< "previous " << A.getPriceOnDate(d, Asset::DatePolicy::PreviousAvailable) << "\n"
		<< "next " << A.getPriceOnDate(d, Asset::DatePolicy::NextAvailable) << "\n";
	std::cout << "exact " << A.getPriceOnDate(d, Asset::DatePolicy::Exact);
 }