#pragma once

// USAGE
// rng.GetNumber() / (ULONG_MAX + 1.0f) = 0.0...1.0

class RNGesus
{
public:
	RNGesus()
		:RNGesus(123, 456, 789)
	{
	}

	RNGesus(const unsigned long& xseed, const unsigned long& yseed, const unsigned long& zseed)
	{
		seed(xseed, yseed, zseed);
	}

	void seed(const unsigned long& xseed, const unsigned long& yseed, const unsigned long& zseed)
	{
		x = xseed;
		y = yseed;
		z = zseed;
	}

	void seed(const unsigned long& xseed)
	{
		x = xseed;
	}

	unsigned long GetNumber()
	{
		x ^= x << 16;
		x ^= x >> 5;
		x ^= x << 1;

		t = x;
		x = y;
		y = z;
		z = t ^ x ^ y;

		return z;
	}

	float GetZeroToOne()
	{
		float result = this->GetNumber() / (ULONG_MAX + 1.0f);
		return result;
	}

private:
	unsigned long x, y, z;
	unsigned long t = 0;
};