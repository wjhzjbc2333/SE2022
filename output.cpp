#include"output.h"
Output::Output(string s) {
	path = s;
	string mkdir = "cd " + s + " && mkdir output";
	system(mkdir.c_str());
	ofstream out1, out2;
	string path1 = s + "output/equal.csv";
	string path2 = s + "output/inequal.csv";
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
		temp += "output/equal.csv";
		out.open(temp.c_str(), ios::app);
	}
	else {
		temp += "output/inequal.csv";
		out.open(temp.c_str(), ios::app);
	}
	if (!out) {
		cout << "文件未打开" << endl;
		exit(-1);
	}
	out << s1.substr(s1.find("input/"), s1.length() - s1.find("input/")) << "," << s2.substr(s2.find("input/"), s2.length() - s2.find("input/")) << '\n';
	out.close();
}