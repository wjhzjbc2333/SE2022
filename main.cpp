#include"main.h"
int main() {
	
	Input input;
	Output out(input.path);
	Judge j;
	for (int i = 0; i < input.allFolderPath.size();i++){
		j.insertDirName(input.allFolderPath[i]);
	}
	j.compare(out);
	
}