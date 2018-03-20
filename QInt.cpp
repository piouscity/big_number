#include "QInt.h"
#include <algorithm>
#include <list>
using namespace std;

/// <summary>
/// Constructor mac dinh cua lop QInt
/// Khoi tao so QInt voi gia tri 0 (tat ca cac bit = 0)
/// </summary>
QInt::QInt()
{
	fill(box, box + 4, 0);
}

/// <summary>
/// Constructor khoi tao so QInt tu mot so int.
/// </summary>
/// <param name = "val"> So int duoc cho </param>
QInt::QInt(int val)
{
	box[0] = val;
	for (int i = 1; i < 4; i++)
		if (val < 0) // MSB = 1
		{
			box[i] = -1; // -1 = 1111 1111 1111 1111 	
		}
		else
		{
			box[i] = 0;
		}
}

/// <summary>
/// Constructor khoi tao so QInt tu mot xau
/// <summary>
/// <param name = "str"> Xau chua so can khoi tao </param>
QInt::QInt(const string &str) : QInt()
{
	QInt tmp(1);
	QInt ten(10);
	for (int i = (int)str.size() - 1; i >= 0; i--)
	{
		if (str[i] == '-')
		{
			*this = -*this;
			break;
		}
		int ch = str[i] - '0';
		*this = (*this) + tmp * QInt(ch);
		tmp = tmp * ten;
	}
}

/// <summary>
/// Ham lay gia tri cua mot bit trong so QInt
/// </summary>
/// <param name = "pos"> Vi tri cua bit can lay gia tri </param>
/// <returns> Gia tri cua bit </returns>
bool QInt::GetBit(int pos) const
{
	int boxId = pos >> 5;
	int bitId = pos & ((1 << 5) - 1);
	return (box[boxId] >> bitId) & 1;
}

/// <summary>
/// Ham dat bit cua so QInt tai mot vi tri cho truoc thanh mot bit cho truoc.
// </summary>
/// <param name = "pos"> Vi tri cua bit can dat gia tri </param>
/// <param name = "bit"> Gia tri cua bit sau khi dat </param>
void QInt::SetBit(int pos, bool bit)
{
	if (GetBit(pos) != bit)
		ChangeBit(pos);
}

/// <summary>
/// Ham thay doi gia tri mot bit tai vi tri cho truoc trong so QInt
/// </summary>
/// <param name = "pos"> Vi tri cua bit can thay doi </param>
void QInt::ChangeBit(int pos)
{
	int boxId = pos >> 5;
	int bitId = pos & ((1 << 5) - 1);
	box[boxId] ^= 1 << bitId;
}

/// <summary>
/// Ham dich bit logic so QInt sang phai.
/// </summary>
/// <param name = "shamt"> So bit can phai dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::operator >> (int shamt) const
{
	if (shamt >= qSize) return QInt(); // Tra ve 0 neu dich qua nhieu.
									   // Thay vi dich so QInt sang phai shamt lan, ta phan
									   // tich shamt = 32 * p + q.
									   // Nhu vay, box thu i se nhan gia tri cua box thu i + p, sau do dich phai them q lan nua.
	int p = shamt >> 5;
	int q = shamt & ((1 << 5) - 1);
	QInt res;
	for (int i = 0; i + p < 4; i++) // box thu i nhan gia tri cua box thu i + p.
		res.box[i] = this->box[i + p];
	for (int i = 0; i < 4; i++) // Dich tung box them q lan nua.
	{
		res.box[i] >>= q; // Dich phai box thu i.
		if (i + 1 < 4)
			res.box[i] |= res.box[i + 1] << (boxSize - q); // Copy 32 - q bit cuoi cua box thu i + 1 de ghep vao 32 - q bit dau cua box thu i.
	}
	return res;
}

/// <summary>
/// Ham dich bit logic so QInt sang trai.
/// </summary>
/// <param name = "shamt"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::operator << (int shamt) const
{
	if (shamt >= qSize) return QInt(); // Tra ve 0 neu dich qua nhieu.
									   // Cach dich tuong tu operator >> 
	int p = shamt >> 5;
	int q = shamt & ((1 << 5) - 1);
	QInt res;
	for (int i = p; i < 4; i++) // box thu i nhan gia tri cua box thu i - p.
		res.box[i] = this->box[i - p];
	for (int i = 3; i >= 0; i--) // Dich tung box them q lan nua.
	{
		res.box[i] <<= q;
		if (i > 0)
			res.box[i] |= this->box[i - 1] >> (boxSize - q); // Copy 32 - q bit cuoi cua box thu i - 1 vao 32 - q bit dau cua box thu i.
	}
	return res;
}

/// <summary>
/// Ham dich bit so hoc so QInt, dich sang trai.
/// </summary>
/// <param name = "shamt"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::AShiftLeft(int shamt) const
{
	bool msb = this->GetBit(qSize - 1); // Luu lai bit trai nhat.
	QInt res = (*this) << shamt; // Dich logic.
	res.SetBit(qSize - 1, msb); // Gan lai bit trai nhat.
	return res;
}

/// <summary>
/// Ham dich bit so hoc so QInt, dich sang phai.
/// </summary>
/// <param name = "shamt"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::AShiftRight(int shamt) const
{
	bool msb = this->GetBit(qSize - 1); // Luu lai bit trai nhat.
	QInt res = (*this) >> shamt; // Dich logic.
	res.SetBit(qSize - 1, msb); // gan lai bit trai nhat.
	return res;
}

/// <summary>
/// Operator hai ngoi &, thuc hien phep toan logic and giua hai so QInt.
/// </summary>
/// <param name = "other"> SO QInt thu hai </param>
/// <returns> Ket qua cua phep and </returns>
QInt QInt::operator & (const QInt &other) const
{
	QInt res;
	for (int i = 0; i < 4; i++)
		res.box[i] = this->box[i] & other.box[i]; // And tung box du lieu.
	return res;
}

/// <summary>
/// Operator hai ngoi |, thuc hien phep toan logic or giua hai so QInt.
/// </summary>
/// <param name = "other"> SO QInt thu hai </param>
/// <returns> Ket qua cua phep or </returns>
QInt QInt::operator | (const QInt &other) const
{
	QInt res;
	for (int i = 0; i < 4; i++)
		res.box[i] = this->box[i] | other.box[i]; // Or tung box du lieu.
	return res;
}

/// <summary>
/// Operator hai ngoi ^, thuc hien phep toan logic xor giua hai so QInt.
/// </summary>
/// <param name = "other"> SO QInt thu hai </param>
/// <returns> Ket qua cua phep xor </returns>
QInt QInt::operator ^ (const QInt &other) const
{
	QInt res;
	for (int i = 0; i < 4; i++)
		res.box[i] = this->box[i] ^ other.box[i]; // Or tung box du lieu.
	return res;
}

/// <summary>
/// Operator mot ngoi ~, thuc hien phep toan logic not mot so QInt.
/// </summary>
/// <returns> Ket qua cua phep not </returns>
QInt QInt::operator ~() const
{
	QInt res;
	for (int i = 0; i < 4; i++)
		res.box[i] = ~this->box[i]; // Not tung box du lieu.
	return res;
}

/// <summary>
/// Operator hai ngoi +, thuc hien phep cong giua hai so QInt.
/// </sumamry>
/// <param name = "other"> So Qint thu hai </param>
/// <returns> Ket qua cua phep cong </returns>
QInt QInt::operator + (const QInt &other) const
{
	QInt res;
	bool carry = false;
	for (int i = 0; i < 4; i++)
	{
		res.box[i] = this->box[i] + other.box[i] + carry; // Cong tung box.
		if ((other.box[i] || carry) == 0) // Ca 2 so = 0
			carry = 0;
		else carry = res.box[i] <= this->box[i]; // Neu tran so thi nho 1 bit sang box tiep theo.
	}
	return res;
}

/// <summary>
/// Operator hai ngoi -, thuc hien phep cong giua hai so QInt.
/// </sumamry>
/// <param name = "other"> So Qint thu hai </param>
/// <returns> Ket qua cua phep tru </returns>
QInt QInt::operator - (const QInt &other) const
{
	QInt res;
	bool carry = false;
	for (int i = 0; i < 4; i++)
	{
		res.box[i] = this->box[i] - (other.box[i] + carry); // Tru tung box.
		if ((other.box[i] || carry) == 0) // Ca 2 so = 0
			carry = 0;
		else carry = res.box[i] >= this->box[i]; // Neu tran so thi nho 1 bit sang box tiep theo.
	}
	return res;
}

/// <summary> 
/// Ham nhan hai so QInt.
/// </summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep nhan </returns>
QInt QInt::operator * (const QInt &other) const
{
	// Nhan 2 so QInt su dung thuat toan Booth.
	QInt A;
	QInt Q = other;
	bool P = false;
	for (int i = 0; i < qSize; i++)
	{
		bool prevP = Q.GetBit(0); // Lay LSB cua so Q.
		if (prevP == false && P == true) // 01.
			A = A + *this;
		if (prevP == true && P == false) // 10.
			A = A - *this;
		P = prevP; // Shift.
		Q = Q >> 1;
		Q.SetBit(qSize - 1, A.GetBit(0));
		A = A.AShiftRight(1);
	}
	// Ket qua la mot so nguyen 32 box co dang A * 2^16 + Q.
	// Tuy nhien, do QInt la so nguyen 16 box nen ta phai loai 16 box thua di, chi lay Q.
	return Q;
}

/// <summary>
/// Operator hai ngoi /, thuc hien phep chia mot so QInt cho mot so QInt.
/// </summary>
/// <param name = "other"> So bi chia </param>
/// <returns> Ket qua phep chia </param>
QInt QInt::operator / (const QInt &other) const
{
	QInt div, mod;
	this->Divide(other, div, mod);
	return div;
}

/// <summary>
/// Ham chia mot so QInt cho mot so QInt khac.
/// </summary>
/// <param name = "other"> So chia </param>
/// <param name = "div"> Ket qua cua phep chia </param>
/// <param name = "mod"> So du cua phep chia </param>
void QInt::Divide(const QInt & other, QInt & div, QInt & mod) const
{
	mod = QInt(0);
	div = *this;
	QInt M = other;
	bool sign = false; // Luu lai dau cua phep chia.
	if (div.GetBit(qSize - 1) == true) // Bao dam div va M luon >= 0
	{
		div = -div;
		sign ^= true;
	}
	if (M.GetBit(qSize - 1) == true)
	{
		M = -M;
		sign ^= true;
	}
	for (int i = 0; i < qSize; i++) // Thuc hien chia theo thuat toan trong tai lieu.
	{
		mod = mod << 1;
		mod.SetBit(0, div.GetBit(qSize - 1)); // Copy MSB cua Q vao LSB cua A.
		div = div << 1;
		QInt tmp = mod - M;
		if (tmp.GetBit(qSize - 1) == false) // tmp >= 0			
		{
			mod = tmp;
			div.SetBit(0, 1);
		}
	}
	if (sign) // Neu thuong la am thi doi dau.
	{
		div = -div;
		mod = -mod;
	}
}

/// <summary>
/// Ham lay so doi cua mot so QInt
/// </summary>
/// <returns> So doi cua so QInt </returns>
QInt QInt::operator - () const
{
	QInt res = ~(*this); // Bu 1	
	return ++res; // Bu 2
}

/// <summary>
/// Toan tu mot ngoi ++ (prefix)
/// </summary>
/// <returns> So QInt sau khi ++ </returns>
QInt QInt::operator ++ ()
{
	bool carry = true;
	for (int i = 0; i < 4; i++)
	{
		uint32_t tmp = this->box[i] + carry;
		carry = tmp < box[i];
		box[i] = tmp;
	}
	return *this;
}

/// <summary>
/// Toan tu mot ngoi -- (prefix)
/// </summary>
/// <returns> So QInt sau khi -- </returns>
QInt QInt::operator -- ()
{
	bool carry = true;
	for (int i = 0; i < 4; i++)
	{
		uint32_t tmp = this->box[i] - carry;
		carry = tmp > box[i];
		box[i] = tmp;
	}
	return *this;
}

/// <summary>
/// Operator hai ngoi >, thuc hien so sanh giua hai so QInt
/// <summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep so sanh </returns>
bool QInt::operator > (const QInt & other) const
{
	if (this->GetBit(qSize - 1) == false && other.GetBit(qSize - 1) == true)
		return true;
	if (this->GetBit(qSize - 1) == true && other.GetBit(qSize - 1) == false)
		return false;
	for (int i = 3; i >= 0; i--)
		if (this->box[i] != other.box[i])
			return this->box[i] > other.box[i];
	return false;
}

/// <summary>
/// Operator hai ngoi <, thuc hien so sanh giua hai so QInt
/// <summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep so sanh </returns>
bool QInt::operator < (const QInt & other) const
{
	return other > *this;
}

/// <summary>
/// Operator hai ngoi >=, thuc hien so sanh giua hai so QInt
/// <summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep so sanh </returns>
bool QInt::operator >= (const QInt & other) const
{
	return !(other > *this);
}

/// <summary>
/// Operator hai ngoi <=, thuc hien so sanh giua hai so QInt
/// <summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep so sanh </returns>
bool QInt::operator <= (const QInt &other) const
{
	return !(*this > other);
}

/// <summary>
/// Operator hai ngoi ==, thuc hien so sanh giua hai so QInt
/// <summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep so sanh </returns>
bool QInt::operator == (const QInt &other) const
{
	for (int i = 0; i < 4; i++)
		if (this->box[i] != other.box[i]) return false;
	return true;
}

/// <summary>
/// Operator hai ngoi !=, thuc hien so sanh giua hai so QInt
/// <summary>
/// <param name = "other"> So QInt thu hai </param>
/// <returns> Ket qua phep so sanh </returns>
bool QInt::operator != (const QInt &other) const
{
	return !(*this == other);
}

/// <summary>
/// Ham xuat mot so QInt ra stream.
/// </summary>
/// <param name = "outf"> Stream output </param>
void QInt::Print(ofstream & outf)
{
	if (*this == QInt(0))
	{
		outf << 0;
		return;
	}
	if (*this < QInt(0)) // Truong hop val < 0
	{
		outf << '-';
		(-*this).Print(outf);
		return;
	}
	QInt zero;
	QInt ten(10);
	QInt tmp = *this;
	list<char> res;
	while (tmp > zero)
	{
		QInt div, mod;
		tmp.Divide(ten, div, mod); // Mod luon < 10.
		res.push_front(mod.box[0] + 48); // Doi sang kieu ki tu.
		tmp = div;
	}
	while (!res.empty()) // Xuat ket qua.
	{
		outf << res.front();
		res.pop_front();
	}
}

/// <summary>
/// Ham trich du lieu tu so QInt sang mang 7 phan tu 2 byte va mot so nguyen 2 byte.
/// <summary>
/// <param name = base> Mang 7 phan tu neu tren </param>
/// <param name = "extend"> So nguyen 2 byte neu tren </param>
void QInt::Expor(uint16_t * base, uint16_t & extend)
{
	for (int i = 0; i < 4; i++)
	{
		base[2 * i] = (box[i] << 16) >> 16; // Copy 16 bit dau.
		if (i < 3)
			base[2 * i + 1] = box[i] >> 16; // Copy 16 bit sau.
		else
			extend = box[i] >> 16;
	}
}

/// <summary>
/// Ham khoi tao mot so QInt tu mot day nhi phan
/// <summary>
/// <param name = "digits"> Day nhi phan duoc cho </param>
/// <returns> So QInt sau khi khoi tao </returns>
/// Throw exception neu nhu do dai day nhi phan qua gioi han
QInt QInt::FromBin(const string & digits)
{
	if ((int)digits.size() > qSize)
		throw _SIZE_EXCEEDED;
	string bit(digits.rbegin(), digits.rend());
	QInt res;
	for (int i = 0; i < (int)bit.size(); i++)
		res.SetBit(i, bit[i] - '0');
	return res;
}

/// <summary>
/// Ham khoi tao mot so QInt tu mot day hexa
/// <summary>
/// <param name = "digits"> Day hexa duoc cho </param>
/// <returns> So QInt sau khi khoi tao </returns>
/// Throw exception neu nhu do dai day nhi phan qua gioi han
QInt QInt::FromHex(const string & digits)
{
	if ((int)digits.size() > hSize)
		throw _SIZE_EXCEEDED;
	string hex(digits.rbegin(), digits.rend());
	QInt res;
	uint32_t tmp = 0;
	// Cu 8 ki tu hexa thi tao thanh mot so nguyen 32 bits.
	// Gom nhom 8 ki tu, tinh gia tri vao bien tam sau do gan vao so QInt.
	for (int i = 0; i < hSize; i++)
	{
		uint32_t val;
		if (i < (int)hex.size())
		{
			val = hex[i] <= '9' ? hex[i] - '0' : hex[i] - 'A' + 10;
		}
		else val = 0;
		tmp |= val << ((i & ((1 << 3) - 1)) << 2); // val << ((i % 8) * 4)
		if ((i & ((1 << 3) - 1)) == 7) // i % 8 = 7
		{
			res.box[i >> 3] = tmp;
			tmp = 0;
		}
	}
	return res;
}

/// <summary>
/// Ham doi mot so QInt sang mot xau nhi phan
/// <summary>
/// <returns> Xau nhi phan sau khi doi </returns>
string QInt::ToBin() const
{
	string res;
	for (int i = 0; i < qSize; i++) res += this->GetBit(i) + '0';
	while (res.size() > 1 && res.back() == '0') res.pop_back();
	reverse(res.begin(), res.end());
	return res;
}

/// <summary>
/// Ham doi mot so QInt sang mot xau hexa.
/// <summary>
/// <returns> Xau hexa sau khi doi </returns>
string QInt::ToHex() const
{
	string res;
	for (int i = 0; i < 4; i++) // Lay tung so nguyen 32 bits thanh phan.
	{
		uint32_t val = this->box[i];
		for (int j = 0; j < 8; j++) // So nguyen 32 bits nay chua 8 ki tu hexa.
		{
			int tmp = val & ((1 << 4) - 1); // Mod 16 lay ra gia tri cua ki tu hexa.
			if (tmp < 10)
				res += tmp + '0';
			else
				res += tmp - 10 + 'A';
			val >>= 4;
		}
	}
	while (res.size() > 1 && res.back() == '0') res.pop_back();
	reverse(res.begin(), res.end());
	return res;
}

/// <summary>
/// Ham dich bit logic so QInt sang phai.
/// </summary>
/// <param name = "shamt"> So bit can phai dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::operator >> (const QInt &shamt) const
{
	if (shamt >= QInt(32)) return QInt(0);
	return (*this) >> shamt.box[0];
}

/// <summary>
/// Ham dich bit logic so QInt sang trai.
/// </summary>
/// <param name = "shamt"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::operator << (const QInt &shamt) const
{
	if (shamt >= QInt(32)) return QInt(0);
	return (*this) << shamt.box[0];
}

/// <summary>
/// Constructor khoi tao mot so QInt tu mot mang 7 + 1 so 16 bit
/// </summary>
/// <param name = "base"> Mang 7 so 16 bit </param>
/// <param name = "extend"> So 16 bit rieng </param>
QInt::QInt(const uint16_t * base, uint16_t extend)
{
	for (int i = 0; i < 4; i++)
	{
		box[i] = base[2 * i]; // Ghep 16 bit dau.
		if (i < 3) // Ghep 16 bit sau.
			box[i] |= uint32_t(base[2 * i + 1]) << 16;
		else
			box[i] |= uint32_t(extend) << 16;
	}
}

/// <summary>
/// Ham tra ve 16 bit cuoi cua so QInt
/// </summary>
/// <returns> 16 bit cuoi cua so QInt </returns>
uint16_t QInt::Expor() const
{
	return box[3] >> 16;
}

/// <summary>
/// Ham tro giup phep nhan 2 so thuc.
/// </summary>
QInt QInt::Multiply(const QInt & other) const
{
	QInt A;
	QInt Q = other;
	bool P = false;
	for (int i = 0; i < qSize; i++)
	{
		bool prevP = Q.GetBit(0); // Lay LSB cua so Q.
		if (prevP == false && P == true) // 01.
			A = A + *this;
		if (prevP == true && P == false) // 10.
			A = A - *this;
		P = prevP; // Shift.
		Q = Q >> 1;
		Q.SetBit(qSize - 1, A.GetBit(0));
		A = A.AShiftRight(1);
	}
	
	return (Q >> 112 | A << 16);
}