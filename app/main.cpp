#include <iostream>
#include <string>

#include "Asset.h"

int main() {
	std::string name = "Apple regular stock";
	std::string ticker = "AAPL";
	Asset A(name, ticker);

	A.readFromFile();
	std::vector<double> A_prices = A.prices();
	std::vector<PriceRecord> A_quotes = A.quotes();
	for (int i = 0; i < A_prices.size(); i++) {
		std::cout << A_quotes[i].date.toString() << " " << A_quotes[i].price << " " << A_prices[i] << "\n";
	}
	return 0;
 }