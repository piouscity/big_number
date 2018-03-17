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
/// <param name = "val"> So QInt can xuat </param>
/// <param name = "outf"> Stream output </param>
void PrintQInt(const QInt & val, ofstream & outf)
{
	if (val == QInt(0))
	{
		outf << 0;
		return;
	}
	if (val < QInt(0)) // Truong hop val < 0
	{
		outf << '-';
		PrintQInt(-val, outf);
		return;
	}
	QInt zero;
	QInt ten(10);
	QInt tmp = val;
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
/// Chuyen doi so QInt thap phan thanh so nhi phan
/// </summary>
/// <param name = "val"> So QInt can chuyen doi </param>
/// <returns> So nhi phan sau khi chuyen doi </returns>
string DecToBin(const QInt &val)
{
	string result = "";

	for (int i = 0; i < qSize; i++)
	{
		result = result + char(val.GetBit(i) + '0');
	}

	while (result.size() > 1 && result.back() == '0')
	{
		result.pop_back();
	}
	return result;
}

/// <summary>
/// Chuyen doi so nhi phan thanh so QInt thap phan
/// </summary>
/// <returns> So QInt thap phan sau khi chuyen doi </returns>
/// Throw exception neu do dai cua day nhi phan qua gioi han
QInt BinToDec(const string &bit)
{
	if (bit.size() > qSize)
		throw _SIZE_EXCEEDED;
	QInt result;
	int length = bit.size();
	for (int i = 0; i < length; i++)
	{
		result.SetBit(i, bit[i] - '0');
	}

	return result;
}

/// <summary>
/// Chuyen doi day nhi phan thanh so thap luc phan
/// </summary>
/// <returns> So thap luc phan sau khi chuyen doi </returns>
/// Throw exception neu do dai cua day nhi phan qua gioi han
string BinToHex(const string &bit)
{		
	if ((int)bit.size() > qSize)
		throw _SIZE_EXCEEDED;
	string result = "";

	int num = 0;
	for (int i = 0, j = 0; i < (int)bit.size(); i++)
	{
		num += (bit[i] - '0') << j;		
		if (j == 3 || i + 1 == (int)bit.size()) // Nhom du 4 bit
		{
			if (num < 10)
			{
				result += char(num + '0');
			}
			else
			{
				result += char(num - 10 + 'A');
			}
			j = num = 0;			
		}
		else j++;
	}	
	return result;
}

/// <summary>
/// Chuyen doi so thap phan QInt thanh so thap luc phan
/// </summary>
/// <returns> So thap luc phan sau khi chuyen doi </returns>
string DecToHex(const QInt &val)
{
	string bit = DecToBin(val);
	string result = BinToHex(bit);
	return result;
}

/// <summary>
/// Chuyen doi so thap luc phan thanh so nhi phan
/// </summary>
/// <returns> So nhi phan sau khi chuyen doi </returns>
/// Throw exception neu do dai cua day nhi phan qua gioi han
string HexToBin(const string &hexa)
{	
	if ((int)hexa.size() > (qSize >> 2))
		throw _SIZE_EXCEEDED;
	string bit = "";

	for (int i = 0; i < (int)hexa.size(); i++) // Lay tung chu cai trong hexa.
	{
		int num;
		if (hexa[i] <= '9') num = hexa[i] - '0';
		else num = hexa[i] - 'A' + 10;

		for (int j = 0; j < 4; j++)
		{
			bit += (num & 1) + '0';
			num >>= 1;
		}
	}
	while (bit.size() > 1 && bit[bit.size() - 1] == '0') bit.pop_back();	
	return bit;
}

/// <summary>
/// Chuyen doi so thap luc phan thanh so QInt thap phan
/// </summary>
/// <returns> So QInt thap phan sau khi chuyen doi </returns>
/// Throw exception neu do dai cua day nhi phan qua gioi han
QInt HexToDec(const string &hexa)
{
	if ((int)hexa.size() > (qSize >> 2))
		throw _SIZE_EXCEEDED;
	string bit = HexToBin(hexa);
	QInt result = BinToDec(bit);
	return result;
}
