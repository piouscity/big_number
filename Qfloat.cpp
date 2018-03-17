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
	{
		//Set giá trị 0
		expo.setsign(0);
		expo.setbias(signbias::UNDER);
		std::fill(val, val + 7, 0);
	}
	else
	{
		expo.setsign(str[0] == '-');
		if (dot == std::string::npos)
			dot = str.length();
		// Nếu trị tuyệt đối quá lớn
		if (dot > head && dot - head - 1 >= signbias::OVER)
		{
			//Set giá trị INF
			expo.setbias(signbias::OVER);
			std::fill(val, val + 7, 0);
		}
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

/* Kiểm tra số có bằng 0? */
bool Qfloat::is_zero() const
{
	if (!expo.underflow()) return 0;
	for (int i = 0; i < 7; ++i)
		if (val[i])
			return 0;
	return 1;
}