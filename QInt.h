#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#ifndef qSize
#define qSize 128
#endif

#ifndef boxSize
#define boxSize 32
#endif

using namespace std;

class QInt
{
private:
	uint32_t box[4];
	void Divide(const QInt &, QInt &, QInt &) const;
public:
	QInt();
	QInt(const bool *);	
	QInt(int);
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
	QInt operator - () const;
	QInt operator ++();
	QInt operator --();	
	bool operator > (const QInt &) const;
	bool operator < (const QInt &) const;
	bool operator >= (const QInt &) const;
	bool operator <= (const QInt &) const;
	bool operator == (const QInt &) const;
	bool operator != (const QInt &) const;
	friend void PrintQInt(const QInt &, ofstream &);
};

void ScanQInt(QInt &);

bool * DecToBin(const QInt &);
QInt BinToDec(const bool *);
char * BinToHex(const bool *);
char * DecToHex(const QInt &);
