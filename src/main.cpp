#include <iostream>

#include "asset/asset.h"

int main() {
	std::string name = "Apple regular stock";
	std::string ticker = "AAPL";
	Asset A(name, ticker);

	A.readPricesFromFile();
	std::vector<double> A_prices = A.prices();
	std::vector<std::string> A_dates = A.dates();
	for (int i = 0; i < A_prices.size(); i++) {
		std::cout << A_dates[i] << " " << A_prices[i] << "\n";
	}
	return 0;
 }