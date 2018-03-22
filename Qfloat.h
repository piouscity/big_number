#pragma once
#include <stdint.h>
#include <string>
#include "QInt.h"
#include "signbias.h"

class Qfloat
{
private:
	signbias expo;
	uint16_t val[7];
public:
	Qfloat();
	Qfloat(const std::string&);
	void set_inf(bool neg);
	void set_nan();
	void set_zero();
	bool is_zero() const;
	bool is_inf() const;
	bool is_nan() const;
	Qfloat operator + (const Qfloat&) const;
	Qfloat operator - (const Qfloat&) const;
	Qfloat operator - () const;
	Qfloat operator * (const Qfloat&) const;
	std::string ToBin() const;
};