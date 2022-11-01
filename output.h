#pragma once
#include<string.h>
#include<iostream>
#include<fstream>
using namespace std;
class Output
{
	string path;

public:

	Output(string);

	void outputToFile(string, string, bool);
};
