#pragma once
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
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

string DecToBin(const QInt &);
QInt BinToDec(const string &);
string BinToHex(const string &);
string DecToHex(const QInt &);
string HexToBin(const string &);
QInt HexToDec(const string &);
