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

#ifndef _DIVIDE_BY_ZERO
#define _DIVIDE_BY_ZERO -1
#endif

#ifndef _SIZE_EXCEEDED
#define _SIZE_EXCEEDED -2
#endif

#ifndef hSize
#define hSize 32
#endif

using namespace std;

class QInt
{
private:
	uint32_t box[4];
	void Divide(const QInt &, QInt &, QInt &) const;
public:
	QInt();
	QInt(int);
	QInt(const string &);
	QInt(const uint16_t *, uint16_t);
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
	void Expor(uint16_t *, uint16_t &);
	uint16_t Expor() const;
	static QInt FromBin(const string &);
	static QInt FromHex(const string &);
	string ToHex() const;
	string ToBin() const;
	void Print(ofstream &);
	QInt operator >> (const QInt &) const;
	QInt operator << (const QInt &) const;
};