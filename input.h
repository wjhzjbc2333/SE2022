#pragma once
#include<vector>
#include<iostream>
#include<dirent.h>
#include<cstring>
using namespace std;
class Input
{

public:
	string path;

	vector<string> allFolderPath;

	Input();

	void getDirName();
};
