#pragma once
#include<vector>
#include<dirent.h>
#include<iostream>
#include"output.h"
using namespace std;
class Judge
{
	vector<string> dirName;
	vector<string> fileName;
public:
	void insertDirName(string s){
		dirName.push_back(s);
	}

	void compare(Output&);

	bool judgeFileEqual(string, string);

	void generateStdIn(string);

	int runCode(string file1, string file2, string path);
};
