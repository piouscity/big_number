#include "Calculator.h"
#include <string>
using namespace std;

/// <summary>
/// Ham thuc hien yeu cau cua nguoi dung.
/// Ham nhan vao mot yeu cau cua nguoi dung, sau do phan loai yeu cau nay
/// duoc thuc hien tren co so nao, sau do goi cac ham thuc hien yeu cau
/// dac trung cho co so do.
/// Ket qua xuat ra stream output.
/// </summary>
/// <param name = "tokens"> Yeu cau cua nguoi dung </param>
/// <param name = "outf"> Stream xuat ket qua </param>
void Calculate(const vector<string> &tokens, ofstream & outf)
{
	if ((int)tokens.size() < 3)
		throw _UNEXPECTED_AGRUMENT;
	if (tokens[0] == "10") // Co so 10.
		Calculate10(tokens, outf);
	else if (tokens[0] == "16") // Co so 16.
		Calculate16(tokens, outf);
	else if (tokens[0] == "2") // Co so 2.
		Calculate2(tokens, outf);
	else
		throw _UNEXPECTED_AGRUMENT;
}

///<summary>
/// Ham thuc hien tinh toan tren co so 10 sau do xuat ra stream output.
/// <summary>
/// <param name = "tokens"> Yeu cau cua nguoi dung </param>
/// <param name = "outf"> Stream output </param>
void Calculate10(const vector<string> & tokens, ofstream & outf)
{
	if ((int)tokens.size() == 3) // Lenh doi co so.
	{		
		if (IsFloatingPoint(tokens[2])) // So thuc
		{
			Qfloat tmp = Qfloat::FromDec(tokens[2]);
			if(tokens[1] == "2") // Chuyen sang nhi phan
				outf << tmp.ToBin();
			else throw _UNEXPECTED_AGRUMENT;
		}
		else
		{
			if (tokens[1] == "2")
				outf << QInt(tokens[2]).ToBin(); // Doi sang nhi phan.
			else if (tokens[1] == "16")
				outf << QInt(tokens[2]).ToHex(); // Doi sang hexa.
			else
				throw _UNEXPECTED_AGRUMENT;
		}		
	}
	else if((int)tokens.size() == 4) // Lenh tinh toan.		
	{		
		if (IsFloatingPoint(tokens[1]) || IsFloatingPoint(tokens[3])) // So thuc
		{
			Qfloat a = Qfloat::FromDec(tokens[1]); // Trich 2 so thuc ra.
			Qfloat b = Qfloat::FromDec(tokens[3]);
			outf << Calculate(a, b, tokens[2]).ToDec(); // Tinh ket qua phep toan chua trong tokens[2] roi xuat.
		}
		else
		{
			QInt a(tokens[1]); // Trich 2 so nguyen ra.
			QInt b(tokens[3]);
			Calculate(a, b, tokens[2]).Print(outf); // Tinh ket qua phep toan chua trong tokens[2] roi xuat.
		}		
	}
	else throw _UNEXPECTED_AGRUMENT;
}

///<summary>
/// Ham thuc hien tinh toan tren co so 2 sau do xuat ra stream output.
/// <summary>
/// <param name = "tokens"> Yeu cau cua nguoi dung </param>
/// <param name = "outf"> Stream output </param>
void Calculate2(const vector<string> & tokens, ofstream & outf)
{
	if ((int)tokens.size() == 3) // Lenh doi co so.
	{		
		if (IsFloatingPoint(tokens[2])) // So thuc
		{
			if (tokens[1] == "10") // Doi sang thap phan
				outf << Qfloat(tokens[2]).ToDec();
			else
				throw _UNEXPECTED_AGRUMENT;
		}
		else
		{
			QInt tmp = QInt::FromBin(tokens[2]); // Khoi tao 1 so QInt tu xau nhi phan.		
			if (tokens[1] == "10") // Doi sang co so 10 thi chi can xuat ra.
				tmp.Print(outf);
			else if (tokens[1] == "16") // Xuat dang hexa.
				outf << tmp.ToHex();
			else
				throw _UNEXPECTED_AGRUMENT;
		}		
	}
	else if ((int)tokens.size() == 4) // Lenh tinh toan.		
	{		
		if (IsFloatingPoint(tokens[1]) || IsFloatingPoint(tokens[3])) // So thuc.
		{
			Qfloat a(tokens[1]); // Trich 2 so thuc ra.
			Qfloat b(tokens[3]);
			outf << Calculate(a, b, tokens[2]).ToBin(); // Tinh ket qua o dang nhi phan roi xuat.
		}
		else
		{
			QInt a = QInt::FromBin(tokens[1]); // Trich 2 so nguyen ra.
			QInt b = QInt::FromBin(tokens[3]);
			outf << Calculate(a, b, tokens[2]).ToBin(); // Tinh toan xong xuat o dang nhi phan.
		}		
	}
	else throw _UNEXPECTED_AGRUMENT;
}

///<summary>
/// Ham thuc hien tinh toan tren co so 16 sau do xuat ra stream output.
/// <summary>
/// <param name = "tokens"> Yeu cau cua nguoi dung </param>
/// <param name = "outf"> Stream output </param>
void Calculate16(const vector<string> & tokens, ofstream & outf)
{
	if ((int)tokens.size() == 3) // Lenh doi co so.
	{
		QInt tmp = QInt::FromHex(tokens[2]); // Khoi tao 1 so QInt tu xau nhi phan.
		if (tokens[1] == "10") // Doi sang co so 10 thi chi can xuat ra.
			tmp.Print(outf);
		else if (tokens[1] == "2") // Doi sang nhi phan.
			outf << tmp.ToBin();
		else
			throw _UNEXPECTED_AGRUMENT;
	}
	else if ((int)tokens.size() == 4) // Lenh tinh toan.		
	{
		QInt a = QInt::FromHex(tokens[1]); // Trich 2 so nguyen ra.
		QInt b = QInt::FromHex(tokens[3]);	
		outf << Calculate(a, b, tokens[2]).ToHex(); // Tinh toan sau do doi sang hexa roi xuat.
	}
	else throw _UNEXPECTED_AGRUMENT;
}

/// <summary>
/// Ham thuc hien phep tinh giua hai so QInt theo phep tinh cho truoc
/// <summary>
/// <param name = "a", "b"> Hai so QInt can thuc hien phep tinh </param>
/// <param name = "ope"> Phep tinh </ope>
/// <returns> Ket qua phep tinh </returns>
QInt Calculate(const QInt & a, const QInt & b, const string & ope)
{
	if (ope == "+")
		return a + b;
	else if (ope == "-")
		return a - b;
	else if (ope == "*")
		return a * b;
	else if (ope == "/")
		return a / b;
	else if (ope == "&")
		return a & b;
	else if (ope == "|")
		return a | b;
	else if (ope == "^")
		return a ^ b;
	else if (ope == ">>")
		return a >> b;
	else if (ope == "<<")
		return a << b;
	else
		throw _UNEXPECTED_AGRUMENT;
}

/// <summary>
/// Ham thuc hien phep tinh giua hai so Qfloat theo phep tinh cho truoc
/// <summary>
/// <param name = "a", "b"> Hai so Qfloat can thuc hien phep tinh </param>
/// <param name = "ope"> Phep tinh </ope>
/// <returns> Ket qua phep tinh </returns>
Qfloat Calculate(const Qfloat & a, const Qfloat & b, const string & ope)
{
	if (ope == "+")
		return a + b;
	else if (ope == "-")
		return a - b;
	else if (ope == "*")
		return a * b;
	else if (ope == "/")
		return a / b;	
	else
		throw _UNEXPECTED_AGRUMENT;
}

/// <summary>
/// Ham kiem tra xem mot so co phai so thuc hay khong
/// </summary>
/// <param name = "val"> So can kiem tra </param>
/// <returns> True neu val la so thuc va nguoc lai </returns>
bool IsFloatingPoint(const string & val)
{
	for (int i = 0; i < (int)val.size(); i++)
		if (val[i] == '.') return true;
	return false;
}
