#include"judge.h"
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
		generateStdIn(path);
		closedir(dirStream);
		for (int j = 0; j < fileName.size();j++){
			for (int k = j + 1; k < fileName.size();k++){
				bool flag = false;
				//int ret = runCode(fileName[j], fileName[k], path);
                //cout << fileName[j] << " " << fileName[k] << endl;
                int ret = runCode(path, fileName[j], fileName[k]);
                if(ret == 1)
					flag = true;
				else if(ret == 0)
					flag = false;
				output.outputToFile(path + fileName[j], path + fileName[k], flag);
                string rmv4 = path + "output1.txt";remove(rmv4.c_str());
		        string rmv5 = path + "output2.txt";remove(rmv5.c_str());
			}
		}
        for (int i = 0; i < 10;i++){
            string rmv1 = path + "input";
            rmv1 += to_string(i);
            rmv1 += ".txt";
            remove(rmv1.c_str());
        }
		string rmv2 = path + "temp1.out";remove(rmv2.c_str());
		string rmv3 = path + "temp2.out";remove(rmv3.c_str());
		string rmv6 = path + "compile1.txt";remove(rmv6.c_str());
		string rmv7 = path + "compile2.txt";remove(rmv7.c_str());
		fileName.clear();
	}
}

void Judge::generateStdIn(string path){
    string format = path + "stdin_format.txt";
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
    for (int i = 0; i < 10;i++){
        string inputFilePath = path + "input" + to_string(i) + ".txt";
        ofstream input(inputFilePath, ios::out);
		if(!input){
			cout << "文件未打开" << endl;
			exit(-1);
		}
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
    }
}

int Judge::runCode(string path, string file1, string file2){
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
	cmd1 += " -o temp1.out 2>> compile1.txt";
	cmd2 += " -o temp2.out 2>> compile2.txt";
	int ret1 = system(cmd1.c_str());
	int ret2 = system(cmd2.c_str());
	for (int i = 0; i < 10;i++){  //十组样例
		string t_cmd1 = "cd ", t_cmd2 = "cd ";
		t_cmd1 += path, t_cmd2 += path;
		t_cmd1 += " && ./temp1.out < input";
        t_cmd1 += to_string(i);
        t_cmd1 += ".txt >> output1.txt";
        t_cmd2 += " && ./temp2.out < input";
        t_cmd2 += to_string(i);
        t_cmd2 += ".txt >> output2.txt";
        int ret1 = system(t_cmd1.c_str());
        int ret2 = system(t_cmd2.c_str());
	}
	if(judgeFileEqual(path + "output1.txt", path + "output2.txt")){	
		return 1;
	}
	return 0;
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
			return false;
		}
	}
	return true;
}
