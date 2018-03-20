#pragma once
#include "Qfloat.h"
#include "QInt.h"
#include <algorithm>

/* Constructor mặc định
	Lưu giá trị 0 */
Qfloat::Qfloat() : expo(0, signbias::UNDER)
{
	std::fill(val, val + 7, 0);
}

/* Constructor chuẩn
	Nhận vào một xâu chứa số nhị phân
	Mỗi kí tự phải nằm trong tập { '1', '0', '-', '.' }
	Kí tự '-' và '.' chỉ xuất hiện nhiều nhất 1 lần
	Kí tự '-' chỉ được xuất hiện ở đầu xâu
*/
Qfloat::Qfloat(const std::string& str)
{
	int head = str.find_first_of('1');
	int dot = str.find_first_of('.');
	if (head == std::string::npos) // Không chứa bit 1
		this->set_zero();
	else
	{
		bool neg = str[0] == '-';
		expo.setsign(neg);
		if (dot == std::string::npos)
			dot = str.length();
		// Nếu trị tuyệt đối quá lớn
		if (dot > head && dot - head - 1 >= signbias::OVER)
			this->set_inf(neg);
		else
		{
			// Nếu không biểu diễn dạng chuẩn được
			if (dot < head && dot - head <= signbias::UNDER)
			{
				head = dot - (signbias::UNDER + 1);
				expo.setbias(signbias::UNDER);
			}
			else if (dot > head)
				expo.setbias(dot - head - 1);
			else
				expo.setbias(dot - head);
			
			// Gán phần trị 
			QInt x;
			int m = 112;
			for (int i = head + 1; i < str.length() && m>0; ++i)
			{
				if (i == dot) continue;
				x.SetBit(--m, str[i] == '1');
			}
			uint16_t temp;
			x.Expor(val, temp);

			if (this->is_zero()) expo.setsign(0);
		}
	}
}

/* Đặt giá trị là -INF nếu neg=TRUE, +INF nếu neg=FALSE */
void Qfloat::set_inf(bool neg)
{
	expo.setsign(neg);
	expo.setbias(signbias::OVER);
	std::fill(val, val + 7, 0);
}

/* Đặt giá trị là NAN */
void Qfloat::set_nan()
{
	expo.setsign(0);
	expo.setbias(signbias::OVER);
	val[0] = 1;
	std::fill(val + 1, val + 7, 0);
}

/* Đặt giá trị là 0 */
void Qfloat::set_zero()
{
	expo.setsign(0);
	expo.setbias(signbias::UNDER);
	std::fill(val, val + 7, 0);
}

/* Kiểm tra số có bằng 0? */
bool Qfloat::is_zero() const
{
	if (!expo.underflow()) return 0;
	for (int i = 0; i < 7; ++i)
		if (val[i])
			return 0;
	return 1;
}

/* Kiểm tra số có phải INF hoặc -INF? */
bool Qfloat::is_inf() const
{
	if (!expo.overflow()) return 0;
	for (int i = 0; i < 7; ++i)
		if (val[i])
			return 0;
	return 1;
}

/* Kiểm tra số có phải NAN? */
bool Qfloat::is_nan() const
{
	return expo.overflow() && !this->is_inf();
}

Qfloat Qfloat::operator+ (const Qfloat& other) const
{
	Qfloat res;
	if (this->is_nan() || other.is_nan())
		res.set_nan();
	else
		if (this->is_inf())
		{
			if (other.is_inf() && this->expo.sign() != other.expo.sign())
				res.set_nan();
			else
				res = *this;
		}
		else if (other.is_inf())
			res = other;
		else
		{
			if (this->is_zero())
				res = other;
			else if (other.is_zero())
				res = *this;
			else
			{
				bool less = this->expo < other.expo;
				signbias this_expo = this->expo;
				signbias other_expo = other.expo;
				QInt this_val(this->val, !this->expo.underflow());
				QInt other_val(other.val, !other.expo.underflow());
				bool brk = 0;
				// Chừng nào phần mũ còn khác nhau
				while (!brk && this_expo != other_expo)
				{
					if (less)
					{
						++this_expo;
						this_val = this_val >> 1;
						if (this_val == 0)
						{
							res = other;
							brk = 1;
						}
					}
					else
					{
						++other_expo;
						other_val = other_val >> 1;
						if (other_val == 0)
						{
							res = *this;
							brk = 1;
						}
					}
				}
				// Nếu chạy tiếp, lúc này phần mũ bằng nhau
				if (!brk)
				{
					QInt res_val;
					res.expo.setbias(this_expo.bias());
					// Cộng phần trị
					if (this_expo.sign() == other_expo.sign())
					{
						res_val = this_val + other_val;
						res.expo.setsign(this_expo.sign());
					}
					else
					if (this_val > other_val)
					{
						res_val = this_val - other_val;
						res.expo.setsign(this_expo.sign());
					}
					else
					{
						res_val = other_val - this_val;
						res.expo.setsign(other_expo.sign());
					}
					// Kiểm tra phần trị
					if (res_val == 0)
						res.set_zero();
					else
					{
						bool brk = 0;
						// Overflow?
						if (res_val.Expor() > 1)
						{
							res_val = res_val >> 1;
							++res.expo;
							if (res.expo.overflow())
							{
								brk = 1;
								res.set_inf(res.expo.sign());
							}
						}
						if (!brk)
						{
							// Không chuẩn
							while (res_val.Expor() == 0)
							{
								res_val = res_val << 1;
								--res.expo;
								if (res.expo.underflow())
								{
									res_val = res_val >> 1;
									break;
								}
							}
							uint16_t temp;
							res_val.Expor(res.val, temp);
						}
					}
				}
			}
		}
	return res;
}

/* */
std::string Qfloat::ToBin() const
{
	if (is_nan()) return "NAN";
	if (is_inf()) return expo.sign() ? "-INF" : "INF";
	if (is_zero()) return "0";

	std::string res;
	if (expo.sign()) res += '-';
	bool dot = 0;
	// Số mũ ko âm
	if (expo.bias() >= 0)
	{
		res += '1';
		QInt v(val, 0);
		int cur = 111;
		for (int i = expo.bias(); i; --i)
		{
			if (cur >= 0) res += v.GetBit(cur--) + '0';
			else res += '0';
		}
		if (cur >= 0)
		{
			dot = 1;
			res += '.';
			for (; cur >= 0; --cur)
				res += v.GetBit(cur) + '0';
		}
	}
	else
	{
		dot = 1;
		res = res + "0.";
		bool unnormal = expo.underflow();
		int ex = unnormal + expo.bias();
		for (; ex < -1; ++ex)
			res += '0';
		res += unnormal ? '0' : '1';

		QInt v(val, 0);
		for(int i=111; i>=0; --i)
			res += v.GetBit(i) + '0';
	}
	if (dot)
		while (res.back() == '0') res.pop_back();
	return res;
}