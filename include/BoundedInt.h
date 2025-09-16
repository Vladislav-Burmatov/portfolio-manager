#ifndef BOUNDEDINT_H
#define BOUNDEDINT_H

#include <string>
#include <iostream>

template <int lower_bound, int higher_bound>
class BoundedInt {
public:
	int value;

	explicit BoundedInt(int v) {
		if (v < lower_bound || v > higher_bound) {
			throw std::runtime_error("Value " + std::to_string(v) + 
				" is outside of range [" + std::to_string(lower_bound) + 
				", " + std::to_string(higher_bound) + "]");
		}
		value = v;
	}

	int get() const { return value; }

	bool operator==(const BoundedInt& other) const { return value == other.value; }

	bool operator!=(const BoundedInt& other) const { return !(*this == other); }

	bool operator<(const BoundedInt& other) const { return value < other.value; }

	bool operator>(const BoundedInt& other) const { return other.value < value; }

	bool operator<=(const BoundedInt& other) const { return !(other.value < value); }

	bool operator>=(const BoundedInt& other) const { return !(value < other.value); }

	friend std::ostream& operator<<(std::ostream& os, const BoundedInt& b) { return os << b.value; }

	friend std::istream& operator>>(std::istream& is, BoundedInt& b) {
		int tmp = 0;
		is >> tmp;

		if (!is) { return is; }

		if (tmp < lower_bound || tmp > higher_bound) {
			is.setstate(std::ios::failbit);
		}
		else { b.value = tmp; }
		return is;
	}
};

#endif