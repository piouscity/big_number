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
	return uint16_t(data << 1) == 0;
}

/* Kiểm tra phần bias chứa toàn bit 1 */
bool signbias::overflow() const
{
	return uint16_t(data << 1) == 65534;
}

/* Trả về true nếu bit dấu là - và ngược lại */
bool signbias::sign() const
{
	return data >= 32768;
}

/* Trả về giá trị của phần bias */
short signbias::bias() const
{
	return short(data & 32767) - 16383;
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

/* Cộng phần bias
Kết quả của phần dấu:
	- nếu 2 toán hạng có phần dấu ngược nhau
	+ nếu 2 toán hạng có phần dấu giống nhau
Throw signbias::OVER nếu overflow
Throw signbias::UNDER nếu underflow
*/
signbias signbias::operator + (const signbias& p) const
{
	short val = this->bias() + p.bias();
	if (val <= -16383) throw UNDER;
	if (val >= 16384) throw OVER;
	return signbias(this->sign() ^ p.sign(), val);
}

/* Trừ phần bias
Kết quả của phần dấu:
	- nếu 2 toán hạng có phần dấu ngược nhau
	+ nếu 2 toán hạng có phần dấu giống nhau
Throw signbias::OVER nếu overflow
Throw signbias::UNDER nếu underflow
*/
signbias signbias::operator - (const signbias& p) const
{
	short val = this->bias() - p.bias();
	if (val <= -16383) throw UNDER;
	if (val >= 16384) throw OVER;
	return signbias(this->sign() ^ p.sign(), val);
}

// Các phương thức dưới đây KHÔNG quan tâm phần dấu:

/* Nếu phần bias đã overflow, phương thức sẽ không thực hiện */
signbias& signbias::operator++()
{
	if (data != 32767 && data != 65535)
		++data;
	return *this;
}
/* Nếu phần bias đã underflow, phương thức sẽ không thực hiện */
signbias& signbias::operator--()
{
	if (data != 0 && data != 32768)
		--data;
	return *this;
}

bool signbias::operator == (const signbias& x) const
{
	return uint16_t(data << 1) == uint16_t(x.data << 1);
}
bool signbias::operator != (const signbias& x) const
{
	return uint16_t(data << 1) != uint16_t(x.data << 1);
}
bool signbias::operator < (const signbias& x) const
{
	return uint16_t(data << 1) < uint16_t(x.data << 1);
}
bool signbias::operator > (const signbias& x) const
{
	return uint16_t(data << 1) > uint16_t(x.data << 1);
}
bool signbias::operator <= (const signbias& x) const
{
	return uint16_t(data << 1) <= uint16_t(x.data << 1);
}
bool signbias::operator >= (const signbias& x) const
{
	return uint16_t(data << 1) >= uint16_t(x.data << 1);
}