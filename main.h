#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<dirent.h>
#include<unistd.h>
#include<cstring>
#include<vector>
#include<time.h>
using namespace std;
class Input {

public:
	string path;

	vector<string> allFolderPath;

	Input();

	void getDirName();
};

class Output {
	string path;

public:

	Output(string);

	void outputToFile(string, string, bool);

};

class Judge {
	vector<string> dirName;
	vector<string> fileName;
public:
	void insertDirName(string s){
		dirName.push_back(s);
	}

	void compare(Output&);

	bool judgeFileEqual(string, string);

	int generateStdInAndRunCode(string, string, string, string);

	int runCode(string file1, string file2, string path);
};
