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
	const static int OVER = 69;
	const static int UNDER = -69;

	signbias();
	signbias(bool neg, short val);
	bool underflow() const;
	bool overflow() const;
	bool sign() const;
	short bias() const;

	void setsign(bool neg);

	signbias& operator++();
	signbias& operator--();
	signbias operator+ (const signbias&) const;
	signbias operator- (const signbias&) const;

	bool operator == (const signbias&) const;
	bool operator != (const signbias&) const;
	bool operator < (const signbias&) const;
	bool operator > (const signbias&) const;
	bool operator <= (const signbias&) const;
	bool operator >= (const signbias&) const;
};