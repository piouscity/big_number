#pragma once
#include <stdint.h>
#include <string>
#include "signbias.h"

class Qfloat
{
private:
	signbias expo;
	uint16_t val[7];
public:
	Qfloat();
	Qfloat(const std::string&);
	bool is_zero() const;
};