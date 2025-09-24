#ifndef BOUNDEDINT_H
#define BOUNDEDINT_H

#include <string>
#include <iostream>

template <int lower_bound, int upper_bound>
class BoundedInt {
private:
	int value_;

public:
	explicit BoundedInt(int v) {
		if (lower_bound > upper_bound) { throw std::runtime_error("Lower bound can not exceed upper bound"); }

		if (v < lower_bound || v > upper_bound) {
			throw std::runtime_error("Value " + std::to_string(v) + 
				" is outside of range [" + std::to_string(lower_bound) + 
				", " + std::to_string(upper_bound) + "]");
		}
		value_ = v;
	}

	int value() const { return value_; }

	bool operator==(const BoundedInt& other) const { return value_ == other.value(); }

	bool operator!=(const BoundedInt& other) const { return !(*this == other); }

	bool operator<(const BoundedInt& other) const { return value_ < other.value(); }

	bool operator>(const BoundedInt& other) const { return other.value() < value_; }

	bool operator<=(const BoundedInt& other) const { return !(other.value() < value_); }

	bool operator>=(const BoundedInt& other) const { return !(value_ < other.value()); }

	friend std::ostream& operator<<(std::ostream& os, const BoundedInt& b) { return os << b.value(); }

	friend std::istream& operator>>(std::istream& is, BoundedInt& b) {
		int tmp = 0;
		is >> tmp;

		if (!is) { return is; }

		if (tmp < lower_bound || tmp > upper_bound) {
			is.setstate(std::ios::failbit);
		}
		else { b.value_ = tmp; }
		return is;
	}
};

#endif