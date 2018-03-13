#include "QInt.h"
#include <algorithm>
using namespace std;

/// <summary>
/// Constructor mac dinh cua lop QInt
/// Khoi tao so QInt voi gia tri 0 (tat ca cac bit = 0)
/// </summary>
QInt::QInt()
{
	fill(bytes, bytes + 4, 0);
}

/// <summary>
/// Constructor khoi tao so QInt tu day nhi phan.
/// </summary>
/// <param name = "bit"> Day nhi phan noi tren </param>
QInt::QInt(const bool * bit)
{
	for (int i = 0; i < qSize; i++)
		SetBit(i, bit[i]);
}

/// <summary>
/// Constructor khoi tao so QInt tu mot so int.
/// </summary>
/// <param name = "val"> So int duoc cho </param>
QInt::QInt(int val)
{
	bytes[0] = val;	
	for (int i = 1; i < 4; i++) 
		if (val < 0) // MSB = 1
		{
			bytes[i] = -1; // -1 = 1111 1111 1111 1111 	
		}
		else 
		{
			bytes[i] = 0;
		}
}

/// <summary>
/// Ham lay gia tri cua mot bit trong so QInt
/// </summary>
/// <param name = "pos"> Vi tri cua bit can lay gia tri </param>
/// <returns> Gia tri cua bit </returns>
bool QInt::GetBit(int pos) const
{
	int byteId = pos >> 5;
	int bitId = pos & ((1 << 5) - 1);
	return (bytes[byteId] >> bitId) & 1;
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
	int byteId = pos >> 5;
	int bitId = pos & ((1 << 5) - 1);
	bytes[byteId] ^= 1 << bitId;
}

/// <summary>
/// Ham dich bit logic so QInt sang phai.
/// </summary>
/// <param name = "shift"> So bit can phai dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::operator >> (int shift) const
{
	if (shift >= qSize) return QInt(); // Tra ve 0 neu dich qua nhieu.
	QInt res;
	for (int i = 0; i < qSize - shift; i++)
		res.SetBit(i, this->GetBit(i + shift));
	return res;
}

/// <summary>
/// Ham dich bit logic so QInt sang trai.
/// </summary>
/// <param name = "shift"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::operator << (int shift) const
{
	if (shift >= qSize) return QInt(); // Tra ve 0 neu dich qua nhieu.
	QInt res;
	for (int i = shift; i < qSize; i++)
		res.SetBit(i, this->GetBit(i - shift));
	return res;
}

/// <summary>
/// Ham dich bit so hoc so QInt, dich sang trai.
/// </summary>
/// <param name = "shift"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::AShiftLeft(int shift) const
{
	bool msb = this->GetBit(qSize - 1); // Luu lai bit trai nhat.
	QInt res = (*this) << shift; // Dich logic.
	res.SetBit(qSize - 1, msb); // Gan lai bit trai nhat.
	return res;
}

/// <summary>
/// Ham dich bit so hoc so QInt, dich sang phai.
/// </summary>
/// <param name = "shift"> So bit can dich </param>
/// <returns> So QInt sau khi dich </returns>
QInt QInt::AShiftRight(int shift) const
{
	bool msb = this->GetBit(qSize - 1); // Luu lai bit trai nhat.
	QInt res = (*this) >> shift; // Dich logic.
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
		res.bytes[i] = this->bytes[i] & other.bytes[i]; // And tung byte du lieu.
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
		res.bytes[i] = this->bytes[i] | other.bytes[i]; // Or tung byte du lieu.
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
		res.bytes[i] = this->bytes[i] ^ other.bytes[i]; // Or tung byte du lieu.
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
		res.bytes[i] = ~this->bytes[i]; // Not tung byte du lieu.
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
		res.bytes[i] = this->bytes[i] + other.bytes[i] + carry; // Cong tung byte.
		if ((other.bytes[i] || carry) == 0) // Ca 2 so = 0
			carry = 0;
		else carry = res.bytes[i] <= this->bytes[i]; // Neu tran so thi nho 1 bit sang byte tiep theo.
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
		res.bytes[i] = this->bytes[i] - (other.bytes[i] + carry); // Tru tung byte.
		if ((other.bytes[i] || carry) == 0) // Ca 2 so = 0
			carry = 0;
		else carry = res.bytes[i] >= this->bytes[i]; // Neu tran so thi nho 1 bit sang byte tiep theo.
	}
	return res;
}

QInt QInt::operator * (const QInt &other) const
{

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
		uint32_t tmp = this->bytes[i] + carry;
		carry = tmp < bytes[i];
		bytes[i] = tmp;
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
		uint32_t tmp = this->bytes[i] - carry;
		carry = tmp > bytes[i];
		bytes[i] = tmp;
	}
	return *this;
}

