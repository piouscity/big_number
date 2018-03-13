#pragma once
#ifndef qSize
#define qSize 128
#endif
class QInt
{
private:
	int bytes[4];
public:
	QInt();
	QInt(const bool *);	
	bool GetBit(int) const;
	void SetBit(int, bool);
	void ChangeBit(int);
	QInt operator >> (int) const;
	QInt operator << (int) const;
	QInt AShiftLeft(int) const;
	QInt AShiftRight(int) const;
	QInt operator + (const QInt &) const;
	QInt operator - (const QInt &) const;
	QInt operator * (const QInt &) const;
	QInt operator / (const QInt &) const;
	QInt operator & (const QInt &) const;
	QInt operator | (const QInt &) const;
	QInt operator ^ (const QInt &) const;
	QInt operator ~ () const;
	bool operator < (const QInt &) const;
	bool operator > (const QInt &) const;
	QInt operator - () const;
	QInt operator + (int) const;
	QInt operator - (int) const;
};

void ScanQInt(QInt &);
void PrintQInt(const QInt &);
bool * DecToBin(const QInt &);
QInt BinToDec(const bool *);
char * BinToHex(const bool *);
char * DecToHex(const QInt &);
