#include <iostream>
#include "QInt.h"
#include <conio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Calculator.h"
using namespace std;

int main(int argc, char ** argv)
{	
	if (argc < 3)
	{
		cout << "Khong du tham so de chay chuong trinh";
		return 0;
	}
	ifstream inf(argv[1], ifstream::in); // File nhap.
	ofstream outf(argv[2], ofstream::out); // FIle xuat.
	string cmd; // Cac dong lenh.
	while (getline(inf, cmd)) // Doc cho den khi het du lieu.
	{		
		stringstream ss; 
		ss << cmd; // Ep cmd vao stringstream.
		vector<string> tokens;
		string t;
		while (ss >> t)
		{
			tokens.push_back(t);
		}				
		Calculate(tokens, outf);		
		outf << '\n';
	}	
	inf.close();
	outf.close();
}