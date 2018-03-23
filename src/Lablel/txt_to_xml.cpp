
#include<iostream>  
#include<fstream>    
#include<string>  

using namespace std;

void 写入TXT(string 名字, string 内容, bool 是否覆盖) {
	ofstream 写入(名字, 是否覆盖 ? ios::trunc : ios::app);

	if (写入.is_open()) {//如果成功的话       
		写入 << 内容 << endl;
		写入.close();
	}
}
int main() {
	fstream 读文件;
	string 目录 = "C:\\Users\\zhumingde\\Desktop\\采集\\thumb\\";
	读文件.open(目录+"Rect.txt");
	string s; int a=220, b=140, c=200, d=200;
//	读文件 >> s;
//	读文件 >> a >> b >> c >> d;
	写入TXT(目录+"train.xml", "<dataset>", 0);
	写入TXT(目录+"train.xml", "<images>", 0);
//	string temp = "<image file='"+s+"'>";
//	写入TXT(目录+"\\train.xml", temp, 0);
//	temp = "<box top='" + to_string(a)+"' left='"+to_string(b)+"' width='"+to_string(c)+"' height='"+to_string(d)+"'/>";
//	写入TXT(目录+"\\train.xml", temp, 0);
	while (!读文件.eof()) {
		读文件 >> s;
		读文件 >> a >> b >> c >> d;
		cout << "进行中..." << endl;
	//	写入TXT(目录+"\\train.xml", "<image>", 0);
		string temp = "<image file='" + s + "'>";
		写入TXT(目录+"train.xml", temp, 0);
		temp = "<box top='" + to_string(b) + "' left='" + to_string(a) + "' width='" + to_string(c) + "' height='" + to_string(d) + "'/>";
		写入TXT(目录+"train.xml", temp, 0);
		写入TXT(目录+"train.xml", "</image>", 0);
	}
	写入TXT(目录+"train.xml", "</images>", 0);
	写入TXT(目录+"train.xml", "</dataset>", 0);
	system("pause");
}