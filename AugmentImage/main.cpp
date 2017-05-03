#include <iostream>
#include <fstream>
#include <direct.h>

using namespace std;

void main()
{
	int a = 10;
	cout << a << endl;
	char buff[1024];
	memset(buff, 0, 1024);
	_getcwd(buff, 1024);
	printf("%s\n", buff);
	//system("pause");
}