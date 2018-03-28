#pragma once
#include "Qfloat.h"
#include <algorithm>
#include <sstream>

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
		expo.setsign(str[0] == '-');
		if (dot == std::string::npos)
			dot = str.length();
		// Nếu trị tuyệt đối quá lớn
		if (dot - head - 1 >= signbias::OVER)
			this->set_inf(expo.sign());
		else
		{
			// Nếu không biểu diễn dạng chuẩn được
			if (dot - head <= signbias::UNDER)
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
			for (int i = head + 1; i < (int)str.length() && m>0; ++i)
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

Qfloat Qfloat::operator - () const
{
	if (this->is_zero() || this->is_nan())
		return *this;
	Qfloat res(*this);
	res.expo.setsign(!this->expo.sign());
	return res;
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
				QInt this_val(this->val, !this->expo.underflow());
				QInt other_val(other.val, !other.expo.underflow());
				signbias this_expo = this->expo;
				signbias other_expo = other.expo;
				bool brk = 0;
				// Chừng nào phần mũ còn khác nhau
				while (!brk && this_expo != other_expo)
				{
					if (less)
					{
						if (!this_expo.underflow())
							this_val = this_val >> 1;
						++this_expo;
						if (this_val == 0)
						{
							res = other;
							brk = 1;
						}
					}
					else
					{
						if (!other_expo.underflow())
							other_val = other_val >> 1;
						++other_expo;
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
						if (res.expo.underflow()) ++res.expo;
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
								--res.expo;
								if (res.expo.underflow())
									break;
								res_val = res_val << 1;
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
Qfloat Qfloat::operator - (const Qfloat& other) const
{
	return operator + (-other);
}

Qfloat Qfloat::operator * (const Qfloat& other) const
{
	Qfloat res;
	if (this->is_nan() || other.is_nan())
		res.set_nan();
	else
		if (this->is_inf() || other.is_inf())
		{
			if (this->is_zero() || other.is_zero())
				res.set_nan();
			else
				res.set_inf(this->expo.sign() ^ other.expo.sign());
		}
		else if (this->is_zero() || other.is_zero())
			res.set_zero();
		else
		{
			res.expo.setsign(this->expo.sign() ^ other.expo.sign());
			int ex = this->expo.bias() + other.expo.bias() 
					+ this->expo.underflow() + other.expo.underflow();
			// Nếu số mũ quá lớn
			if (ex >= signbias::OVER)
				res.set_inf(res.expo.sign());
			else
			{
				
				QInt this_val(this->val, !this->expo.underflow());
				QInt other_val(other.val, !other.expo.underflow());
				QInt extend;
				QInt res_val = this_val.Multiply(other_val, extend);
				if (ex <= signbias::UNDER)
				{
					// Gọi res_val.Expor() là e, lúc này 0 <= e <= 3
					int shift = signbias::UNDER + 1 - ex; // shift > 0
					res_val = res_val >> shift; // Do đó, 0 <= e <= 1
					res.expo.setbias(signbias::UNDER + res_val.Expor());
					uint16_t temp;
					res_val.Expor(res.val, temp);
					if (res.is_zero()) res.expo.setsign(0);
				}
				else
				{
					res.expo.setbias(ex);
					bool brk = 0;
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
						int head = 111;
						while (res_val.Expor() == 0)
						{
							--res.expo;
							if (res.expo.underflow())
								break;
							res_val = res_val << 1;
							if (head >= 0)
								res_val.SetBit(0, extend.GetBit(head--));
						}
						uint16_t temp;
						res_val.Expor(res.val, temp);
					}
				}
			}
		}
	return res;
}

Qfloat Qfloat::operator / (const Qfloat& other) const
{
	Qfloat res;
	if (this->is_nan() || other.is_nan()) // Neu mot trong 2 so la nan thi tra ve nan.
	{
		res.set_nan();
		return res;
	}
	if (other.is_inf())
	{
		if (this->is_inf()) // oo / oo = nan.
			res.set_nan();
		else // x / oo = 0.
			res.set_zero();
		return res;
	}
	if (other.is_zero()) // So chia = 0 thi tra ve nan.
	{
		res.set_nan();
		return res;
	}
	if (this->is_inf()) // Neu so bi chia la +oo hoac -oo.
	{
		if (other.expo.sign() == this->expo.sign()) // Chia 2 so cung dau thi ra +oo.
			res.set_inf(0);
		else // Chia 2 so khac dau thi ra -oo.
			res.set_inf(1);
		return res;
	}
	if (this->is_zero()) return Qfloat();
	
	res.expo.setsign(this->expo.sign() ^ other.expo.sign());
	int ex = this->expo.bias() - other.expo.bias()
		+ this->expo.underflow() - other.expo.underflow();

	QInt this_val(this->val, !this->expo.underflow());
	QInt other_val(other.val, !other.expo.underflow());
	QInt res_val = this_val / other_val;
	this_val = this_val % other_val;
	if (res_val > 0)
	{
		// Số 1 trái nhất
		unsigned head = 112u;
		while (res_val.GetBit(head) == 0) --head;
		ex += head;
		while (head < 112u)
		{
			bool temp = this_val.Divide(other_val);
			res_val = res_val << 1;
			res_val.SetBit(0, temp);
			++head;
		}
	}
	else
	{
		while (1)
		{
			bool temp = this_val.Divide(other_val);
			--ex;
			if (temp) break;
		}
		res_val.SetBit(112, 1);
		for (int i = 111; i>=0; --i)
			res_val.SetBit(i, this_val.Divide(other_val));
	}
	if (ex >= signbias::OVER) // Quá lớn
		res.set_inf(res.expo.sign());
	else
	{
		if (ex <= signbias::UNDER)
		{
			int shift = signbias::UNDER + 1 - ex;
			res_val = res_val >> shift;
			res.expo.setbias(signbias::UNDER);
		}
		else
			res.expo.setbias(ex);
		uint16_t temp;
		res_val.Expor(res.val, temp);
		if (res.is_zero()) res.expo.setsign(0);
	}
	return res;
}

Qfloat Qfloat::operator << (uint16_t shift) const
{
	if (this->is_nan() || this->is_zero()) return *this;
	Qfloat res = *this;
	QInt res_val(res.val, !res.expo.underflow());
	while (shift && res.expo.underflow())
	{
		--shift;
		res_val = res_val << 1;
		if (res_val.Expor() > 0) ++res.expo;
	}
	int ex = res.expo.bias() + shift;
	if (ex >= signbias::OVER) res.set_inf(res.expo.sign());
	else
	{
		res.expo.setbias(ex);
		uint16_t temp;
		res_val.Expor(res.val, temp);
	}
	return res;
}
Qfloat Qfloat::operator >> (uint16_t shift) const
{
	if (this->expo.overflow() || this->is_zero()) return *this;
	Qfloat res = *this;
	int ex = res.expo.bias() - shift;
	if (ex > signbias::UNDER)
		res.expo.setbias(ex);
	else
	{
		QInt res_val(res.val, !res.expo.underflow());
		if (!res.expo.underflow())
			shift -= res.expo.bias() - (signbias::UNDER + 1);
		res.expo.setbias(signbias::UNDER);
		res_val = res_val >> shift;
		uint16_t temp;
		res_val.Expor(res.val, temp);
	}
	return res;
}

bool Qfloat::operator < (const Qfloat& other) const
{
	if (this->expo.sign() != other.expo.sign())
		return this->expo.sign();
	bool neg = this->expo.sign();
	if (this->expo != other.expo)
		return neg ? this->expo > other.expo : this->expo < other.expo;
	for (int i = 6; i>=0; --i)
		if (this->val[i] != other.val[i])
			return neg ? this->val[i] > other.val[i] : this->val[i] < other.val[i];
	return 0;
}

/* Trả về xâu thể hiện nhị phân */
std::string Qfloat::ToBin() const
{
	if (is_nan()) return "NAN";
	if (is_inf()) return expo.sign() ? "-INF" : "INF";
	if (is_zero()) return "0";

	std::string res;
	if (expo.sign()) res += '-';

	if (std::abs(expo.bias()) > 120)
	{
		res += expo.underflow() ? '0' : '1';
		res += '.';
		QInt v(val, 0);
		for (int i = 111; i>=0; --i)
			res += v.GetBit(i) + '0';
		while (res.back() == '0') res.pop_back();
		if (res.back() == '.') res += '0';
		res += "*2^";
		int ex = expo.bias() + expo.underflow();
		stringstream ss;
		string temp;
		ss << ex;
		ss >> temp;
		res = res + temp;
	}
	else
	{
		bool dot = 0;
		// Số mũ ko âm
		if (expo.bias() >= 0)
		{
			res += '1';
			QInt v(val, 0);
			int cur = 111;
			for (unsigned i = expo.bias(); i; --i)
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
			bool denormal = expo.underflow();
			unsigned ex = -(denormal + expo.bias());
			for (; ex > 1u; --ex)
				res += '0';
			res += denormal ? '0' : '1';

			QInt v(val, 0);
			for (int i = 111; i >= 0; --i)
				res += v.GetBit(i) + '0';
		}
		if (dot) {
			while (res.back() == '0') res.pop_back();
			if (res.back() == '.') res += '0';
		}
	}
	return res;
}

Qfloat Qfloat::FromDec(const string & val)
{
	int dot = val.find_first_of('.');
	if (dot == string::npos) dot = val.length();
	Qfloat res;
	Qfloat ten("1010");
	for (int i = (int)val.length() - 1; i > dot; --i)
	{
		res = res + naturalDigits[val[i] - '0'];
		res = res / ten;
	}
	Qfloat pow("1");
	for (int i = dot - 1; i >= 0 && val[i] != '-'; --i)
	{
		res = res + naturalDigits[val[i] - '0'] * pow;
		pow = pow * ten;
	}
	if (val[0] == '-') res = -res;
	return res;
}

string Qfloat::ToDec() const
{
	if (is_nan()) return "NAN";
	if (is_inf()) return expo.sign() ? "-INF" : "INF";
	if (is_zero()) return "0";

	string res;
	Qfloat ten("1010"), num(*this);
	if (expo.sign())
	{
		res += '-';
		num = -num;
	}
	
	// Phần nguyên
	if (num < naturalDigits[1]) res += '0';
	else
	{
		Qfloat pow = naturalDigits[1], ptmp;
		int cnt = 0;
		while (1)
		{
			ptmp = pow * ten;
			if (num < ptmp) break;
			pow = ptmp;
			++cnt;
		}
		for(; cnt>=0; --cnt)
		{
			ptmp = num / pow;
			if (ptmp < naturalDigits[1]) res += '0';
			else
			{
				int str = 1;
				uint16_t tmp = ptmp.val[6];
				for (unsigned ex = ptmp.expo.bias(); ex; --ex) {
					(str <<= 1) |= tmp >> 15;
					tmp <<= 1;
				}
				res += str + '0';
				num = num - pow * naturalDigits[str];
			}
			pow = pow / ten;
		}
	}
	res += '.';
	// Phần thực
	num = num * ten;
	for(unsigned cnt = res.length() - (res[0]=='-') - 1; cnt<32; ++cnt)
	{
		if (num < naturalDigits[1]) res = res + '0';
		else
		{
			int str = 1;
			uint16_t tmp = num.val[6];
			for (unsigned ex = num.expo.bias(); ex; --ex) {
				(str <<= 1) |= tmp >> 15;
				tmp <<= 1;
			}
			res += str + '0';
			num = num - naturalDigits[str];
		}
		num = num * ten;
	}
	// Làm tròn
	if (res.back() != '.')
	{
		bool carry = res.back() >= '5';
		res.pop_back();
		while (carry && res.back() != '.')
		{
			if (res.back() < '9') {
				++res.back();
				carry = 0;
			}
			else
				res.pop_back();
		}
	}

	while (res.back() == '0') res.pop_back();
	if (res.back() == '.') res += '0';
	return res;
}
