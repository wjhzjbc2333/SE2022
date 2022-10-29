#include"main.h"
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

void Judge::compare(Output &output){
	for (int i = 0; i < dirName.size();i++){
		char tempDirName[1024];
		strcpy(tempDirName, dirName[i].c_str());
		DIR* dirStream;
		dirStream = opendir(tempDirName);
		struct dirent* dirInfo;
		while((dirInfo = readdir(dirStream)) != 0){
			if(dirInfo->d_name[0] == '.') continue;
			string temp = dirInfo->d_name;
			if(temp.find(".c") != temp.npos){
				fileName.push_back(temp);
			}
		}
		string format = dirName[i];
		string path = format;
		format += "stdin_format.txt";
		//generateStdIn(format, path);
		closedir(dirStream);
		for (int j = 0; j < fileName.size();j++){
			for (int k = j + 1; k < fileName.size();k++){
				bool flag = false;
				//int ret = runCode(fileName[j], fileName[k], path);
				int ret = generateStdInAndRunCode(format, path, fileName[j], fileName[k]);
				if(ret == 1)
					flag = true;
				else if(ret == 0)
					flag = false;
				output.outputToFile(path + fileName[j], path + fileName[k], flag);
			}
		}
		//string rmv1 = path + "input.txt";remove(rmv1.c_str());
		//string rmv2 = path + "temp1.out";remove(rmv2.c_str());
		//string rmv3 = path + "temp2.out";remove(rmv3.c_str());
		//string rmv4 = path + "output1.txt";remove(rmv4.c_str());
		//string rmv5 = path + "output2.txt";remove(rmv5.c_str());
		fileName.clear();
	}
}

int Judge::generateStdInAndRunCode(string format, string path, string file1, string file2){
	ifstream formatIn(format, ios::in);
	if(!formatIn){
		cout << "文件未打开" << endl;
		exit(-1);
	}
	string formatTemp;
	vector<pair<char, pair<int, int>>> myFormat;
	while(!formatIn.fail()){
		formatIn >> formatTemp;
		if(formatTemp.find("int")!=formatTemp.npos){
			int lower = stoi(formatTemp.substr(formatTemp.find("(") + 1, formatTemp.find(",") - formatTemp.find("(") - 1));
			int upper = stoi(formatTemp.substr(formatTemp.find(",") + 1, formatTemp.find(")") - formatTemp.find(",") - 1));
			myFormat.push_back(make_pair('i', make_pair(lower, upper)));
		}
		else if(formatTemp.find("char")!=formatTemp.npos){
			myFormat.push_back(make_pair('c', make_pair(0, 25)));
		}
		else if(formatTemp.find("string")!=formatTemp.npos){
			int lower = stoi(formatTemp.substr(formatTemp.find("(") + 1, formatTemp.find(",") - formatTemp.find("(") - 1));
			int upper = stoi(formatTemp.substr(formatTemp.find(",") + 1, formatTemp.find(")") - formatTemp.find(",") - 1));
			myFormat.push_back(make_pair('s', make_pair(lower, upper)));
		}
	}
	formatIn.close();
	string inputFilePath = path + "input.txt";
	/*
	ofstream input(inputFilePath, ios::app);
	if(!input){
		cout << "文件未打开" << endl;
		exit(-1);
	}
	*/
	string cmd1 = "cd ", cmd2 = "cd ";
	cmd1 += path, cmd2 += path;
	if(file1.find(".cpp") != file1.npos) {
		cmd1 += " && g++ ";
	}
	else{
		cmd1 += " && gcc ";
	}
	if(file2.find(".cpp") != file2.npos) {
		cmd2 += " && g++ ";
	}
	else{
		cmd2 += " && gcc ";
	}
	cmd1 += file1;
	cmd2 += file2;
	cmd1 += " -o temp1.out";
	cmd2 += " -o temp2.out";
	char c_cmd1[256], c_cmd2[256];
	strcpy(c_cmd1, cmd1.c_str());
	strcpy(c_cmd2, cmd2.c_str());
	int ret1 = system(c_cmd1);
	int ret2 = system(c_cmd2);
	for (int i = 0; i < 10;i++){  //十组样例
		ofstream input(inputFilePath, ios::out);
		if(!input){
			cout << "文件未打开" << endl;
			exit(-1);
		}
		//其中一组
		for (int j = 0; j < myFormat.size() - 1;j++){
			//srand((unsigned long)time(0));
			int lower = myFormat[j].second.first;
			int upper = myFormat[j].second.second;
			if(myFormat[j].first == 'i'){
				int result = rand() % (upper - lower) + lower;
				input << result << " ";
			}
			else if(myFormat[j].first == 'c'){
				char result = 'a';
				result += rand() % 26;
				input << result << " ";
			}
			else if(myFormat[j].first == 's'){
				int num = rand() % (upper - lower) + lower;
				string result = "";
				for (int i = 0; i < num;i++){
					char c = 'a';
					c += rand() % 26;
					result += c;
				}
				input << result << " ";
			}
		}
		input.close();

		string t_cmd1 = "cd ", t_cmd2 = "cd ";
		t_cmd1 += path, t_cmd2 += path;
		t_cmd1 += " && ./temp1.out < input.txt & >> output1.txt";
		t_cmd2 += " && ./temp2.out < input.txt & >> output2.txt";
		char tc_cmd1[256], tc_cmd2[256];
		strcpy(tc_cmd1, t_cmd1.c_str());
		strcpy(tc_cmd2, t_cmd2.c_str());
		int ret1 = system(tc_cmd1);
		int ret2 = system(tc_cmd2);
		//input << '\n';
	}
	if(judgeFileEqual(path + "output1.txt", path + "output2.txt")){	
		return 1;
	}
	return 0;
	//input.close();
}

int Judge::runCode(string file1, string file2, string path){
	string cmd1 = "cd ", cmd2 = "cd ";
	cmd1 += path, cmd2 += path;
	if(file1.find(".cpp") != file1.npos) {
		cmd1 += " && g++ ";
	}
	else{
		cmd1 += " && gcc ";
	}
	if(file2.find(".cpp") != file2.npos) {
		cmd2 += " && g++ ";
	}
	else{
		cmd2 += " && gcc ";
	}
	cmd1 += file1;
	cmd2 += file2;
	cmd1 += " -o temp1.out && ./temp1.out < input.txt > output1.txt";
	cmd2 += " -o temp2.out && ./temp2.out < input.txt > output2.txt";
	char c_cmd1[256], c_cmd2[256];
	strcpy(c_cmd1, cmd1.c_str());
	strcpy(c_cmd2, cmd2.c_str());
	int ret1 = system(c_cmd1);
	int ret2 = system(c_cmd2);
	if(ret1 != 0 || ret2 != 0){
		return 2;
	}
	else{
		
		if(judgeFileEqual(path + "output1.txt", path + "output2.txt")){	
			return 1;
		}
		else{
			return 0;
		}
	}
}

bool Judge::judgeFileEqual(string s1, string s2) {
	ifstream in1, in2;
	string str1, str2;
	in1.open(s1, ios::in);
	in2.open(s2, ios::in);
	if (!in1 || !in2) {
		cout << "文件未打开" << endl;
		exit(-1);
	}
	while (!in1.fail() && !in2.fail()) {
		in1 >> str1;
		in2 >> str2;
		if (str1 != str2) {
			cout << str1 << endl << str2;
			return false;
		}
	}
	return true;
}

Output::Output(string s) {
	path = s;
	ofstream out1, out2;
	string path1 = s + "equal.csv";
	string path2 = s + "inequal.csv";
	out1.open(path1.c_str(), ios::out);
	out2.open(path2.c_str(), ios::out);
	if (!out1 || !out2) {
		cout << "文件未打开" << endl;
		exit(-1);
	}
	else {
		out1 << "file1,file2" << '\n';
		out2 << "file1,file2" << '\n';
		out1.close();
		out2.close();
	}
}

void Output::outputToFile(string s1, string s2, bool equal) {
	ofstream out;
	string temp = path;
	if (equal) {
		temp += "equal.csv";
		out.open(temp.c_str(), ios::app);
	}
	else {
		temp += "inequal.csv";
		out.open(temp.c_str(), ios::app);
	}
	if (!out) {
		cout << "文件未打开" << endl;
		exit(-1);
	}
	out << s1.substr(s1.find("input/"), s1.length() - s1.find("input/")) << "," << s2.substr(s2.find("input/"), s2.length() - s2.find("input/")) << '\n';
	out.close();
}

int main() {
	Input input;
	Output out(input.path);
	Judge j;
	for (int i = 0; i < input.allFolderPath.size();i++){
		j.insertDirName(input.allFolderPath[i]);
	}
	j.compare(out);
}