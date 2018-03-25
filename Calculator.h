#pragma once
#include <fstream>
#include "QInt.h"
#include "Qfloat.h"
#ifndef _UNEXPECTED_AGRUMENT
#define _UNEXPECTED_AGRUMENT -1
#endif

#include <vector>
using namespace std;

void Calculate(const vector<string> &, ofstream &);
void Calculate10(const vector<string> &, ofstream &);
void Calculate2(const vector<string> &, ofstream &);
void Calculate16(const vector<string> &, ofstream &);
QInt Calculate(const QInt &, const QInt &, const string &);
Qfloat Calculate(const Qfloat &, const Qfloat &, const string &);
bool IsFloatingPoint(const string &);