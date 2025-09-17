#include <iostream>
#include <string>

#include "Asset.h"

int main() {

	/*Date d1(8, 31, 2025);
	Date d2(8, 29, 2025);
	Date d3(9, 2, 2025);
	std::cout << d1.daysSinceZero() << " " << d2.daysSinceZero() << " " << d3.daysSinceZero() << " ";*/

	Date d(9, 12, 2015);
	Date d1(8, 29, 2025);
	Date d2(9, 2, 2025);

	std::string name = "Apple regular stock";
	std::string ticker = "AAPL";
	Asset A(name, ticker);
	A.loadFromFile();

	std::cout << ticker << " " << name << "\n";
	std::cout << A.getFirstDate().toString() << " " << A.getLastDate().toString() << "\n";
	std::cout << A.getHistoryLength() << " quotes \n";

	std::cout << "Distanceeeees: " << d.daysBetween(d1) << " " << d.daysBetween(d2) << "\n";

	try { std::cout << "closest " << A.getPriceOnDate(d, Asset::DatePolicy::Closest) << "\n"; }
	catch (std::runtime_error) { std::cout << "couldn't find closest for " + d.toString() + "\n"; }

	try { std::cout << "previous " << A.getPriceOnDate(d, Asset::DatePolicy::PreviousAvailable) << "\n"; }
	catch (std::runtime_error) { std::cout << "couldn't find previous for " + d.toString() + "\n"; }

	try { std::cout << "next " << A.getPriceOnDate(d, Asset::DatePolicy::NextAvailable) << "\n"; }
	catch (std::runtime_error) { std::cout << "couldn't find next for " + d.toString() + "\n"; }
	
	try { std::cout << "exact " << A.getPriceOnDate(d, Asset::DatePolicy::Exact) << "\n"; }
	catch (std::runtime_error) { std::cout << "couldn't find exact for " + d.toString() + "\n"; }
 }