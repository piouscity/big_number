#pragma once
#include <stdint.h>

/*Đối tượng này gồm 2 phần:
	Phần dấu của số thực
	Phần mũ (phần bias) của số thực
*/
class signbias
{
private:
	uint16_t data;
public:
	const static int OVER = 16384;
	const static int UNDER = -16383;

	signbias();
	signbias(bool neg, short val);
	bool underflow() const;
	bool overflow() const;
	bool sign() const;
	int bias() const;

	void setsign(bool neg);
	void setbias(short val);

	signbias& operator++();
	signbias& operator--();

	bool operator == (const signbias&) const;
	bool operator != (const signbias&) const;
	bool operator < (const signbias&) const;
	bool operator > (const signbias&) const;
	bool operator <= (const signbias&) const;
	bool operator >= (const signbias&) const;
};