#include"input.h"
Input::Input(){
	string temp;
	cout << "请输入Input文件夹完整路径:";
	cin >> temp;
	path = temp;
	if(path[path.length()]!='/')
		path += '/';
	getDirName();
}

void Input::getDirName(){
	char inputPath[128];
	strcpy(inputPath, path.c_str());
	DIR* dirStream;
	dirStream = opendir(inputPath);
	struct dirent* dirInfo;
	while((dirInfo = readdir(dirStream)) != 0) {
		string temp = dirInfo->d_name;
		if(temp.find(".")!=temp.npos) continue;
		allFolderPath.push_back(path + temp + '/');
	}
}
