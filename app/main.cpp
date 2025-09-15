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
	Date s(8, 27, 2025);
	Date e(9, 3, 2025);
	std::cout << s.toString() << " " << e.toString() << "\n";
	double y;
	y = A.getYields(s, e);
	std::cout << "\n" << y << "\n";
	return 0;
 }