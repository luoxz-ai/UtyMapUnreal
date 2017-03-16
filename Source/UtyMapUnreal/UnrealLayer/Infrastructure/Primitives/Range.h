#pragma once

#include <string>
#include <type_traits>

// Represents range
// IComparable type
template<typename T>
class TLodRange
{
public:
	// Minimum value of the range.
	const T Minimum;
	// Maximum value of the range.
	const T Maximum;

	// Creates instance of TLodRange
	// @param Minimum value of the range
	// @param Maximum value of the range
	TLodRange(T min, T max) : Minimum(min), Maximum(max)
	{
	}

// Determines if the range is valid.
// @return True if range is valid, else false
bool IsValid()
{
	return Minimum.compare(Maximum) <= 0;
}

// Determines if the provided value is inside the range.
// @param value">The value to test
// @return True if the value is inside Range, else false
bool Contains(T value)
{
	return (Minimum <= value) && (value <= Maximum);
}

// Determines if this Range is inside the bounds of another range.
// @param range - The parent range to test on
// @return True if range is inclusive, else false
bool IsInside(TLodRange<T> range)
{
	return IsValid() && range.IsValid() && range.Contains(Minimum) && range.Contains(Maximum);
}

// Determines if another range is inside the bounds of this range.
// @param range - The child range to test
// @return True if range is inside, else false
bool Contains(TLodRange<T> range)
{
	return IsValid() && range.IsValid() && Contains(range.Minimum) && Contains(range.Maximum);
}

// Presents the Range in readable format.
// @return String representation of the Range
virtual std::string ToString()
{
	//return std::string::Format(std::string("[{0} - {1}]"), Minimum, Maximum);
	return std::string("");
}
};