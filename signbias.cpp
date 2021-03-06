#include "signbias.h"

/*Constructor mặc định
	Bit dấu là +
	Phần bias là 0
*/
signbias::signbias() : data(16383) // 0011111111111111
{}

/*Constructor
	neg = true nếu bit dấu là - và ngược lại
	val là giá trị của phần bias
(Hãy đảm bảo rằng -16383<=val<=16384)
*/
signbias::signbias(bool neg, short val) : 
data((neg ? 1 << 15 : 0) | (val + 16383))
{}

/* Kiểm tra phần bias chứa toàn bit 0 */
bool signbias::underflow() const
{
	return (data & 32767) == 0;
}

/* Kiểm tra phần bias chứa toàn bit 1 */
bool signbias::overflow() const
{
	return (data & 32767) == 32767;
}

/* Trả về true nếu bit dấu là - và ngược lại */
bool signbias::sign() const
{
	return data >= 32768;
}

/* Trả về giá trị của phần bias */
int signbias::bias() const
{
	return (data & 32767) - 16383;
}

/* Gán lại phần dấu. neg = TRUE tức là dấu - và ngược lại */
void signbias::setsign(bool neg)
{
	if (neg) data |= 32768;
	else data &= 32767;
}

/* Gán lại phần bias với giá trị val.
	Hãy đảm bảo rằng -16383<=val<=16384 */
void signbias::setbias(short val)
{
	(data &= 32768) |= (val + 16383);
}

// Các phương thức dưới đây KHÔNG quan tâm phần dấu:

/* Nếu phần bias đã overflow, phương thức sẽ không thực hiện */
signbias& signbias::operator++()
{
	if (!overflow())
		++data;
	return *this;
}
/* Nếu phần bias đã underflow, phương thức sẽ không thực hiện */
signbias& signbias::operator--()
{
	if (!underflow())
		--data;
	return *this;
}

bool signbias::operator == (const signbias& x) const
{
	return uint16_t(data & 32767) == uint16_t(x.data & 32767);
}
bool signbias::operator != (const signbias& x) const
{
	return uint16_t(data & 32767) != uint16_t(x.data & 32767);
}
bool signbias::operator < (const signbias& x) const
{
	return uint16_t(data & 32767) < uint16_t(x.data & 32767);
}
bool signbias::operator > (const signbias& x) const
{
	return uint16_t(data & 32767) > uint16_t(x.data & 32767);
}
bool signbias::operator <= (const signbias& x) const
{
	return uint16_t(data & 32767) <= uint16_t(x.data & 32767);
}
bool signbias::operator >= (const signbias& x) const
{
	return uint16_t(data & 32767) >= uint16_t(x.data & 32767);
}