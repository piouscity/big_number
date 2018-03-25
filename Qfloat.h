#pragma once
#include <stdint.h>
#include <string>
#include "QInt.h"
#include "signbias.h"
using namespace std;

class Qfloat
{
private:
	signbias expo;
	uint16_t val[7];
public:
	Qfloat();
	Qfloat(const std::string&);
	static Qfloat FromDec(const string &);
	static Qfloat FromBin(const string &);
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
	Qfloat operator / (const Qfloat&) const;
	Qfloat operator << (uint16_t) const;
	Qfloat operator >> (uint16_t) const;
	bool operator < (const Qfloat&) const;
	string ToBin() const;
	string ToDec() const;	
};

const Qfloat naturalDigits[10] =
{ Qfloat("0"), Qfloat("1"), Qfloat("10"), Qfloat("11"), Qfloat("100"), Qfloat("101"), Qfloat("110"), Qfloat("111"), Qfloat("1000"), Qfloat("1001") };
