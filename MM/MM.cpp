#include <iostream>
#include "Memory.h"
#include "stdlib.h"
using namespace std;

char* str;
int h;

int main()
{
	Init();
	h = AllocMem(10);
	cout << "handle: " << h << endl;
	Dump();
	WriteMem(2, 0, 10, "Hello world!");
	Dump();
	str = new char[3];
	ReadMem(2, 0, 3, str);
	cout << "Read: ";
	int i = 0;
	while (str[i])
	{
		cout << str[i];
		i++;
	}
	cout << endl;
	h = AllocMem(23);
	Dump();
	WriteMem(h, 0, 10, "Buy");
	Dump();
	h = AllocMem(100);
	Dump();
	WriteMem(h, 3, 4, "nice");
	h = AllocMem(200);
	Dump();
	WriteMem(h, 5, 3, "UU");
	h = AllocMem(3000);
	Dump();
	WriteMem(h, 9, 10, "SOS VOPROS");
	Dump();
	FreeMem(2);
	FreeMem(3);
	Dump();
	h = AllocMem(6667);
	Dump();
	h = AllocMem(40);
	Dump();
	h = AllocMem(32);
	Dump();
	FreeMem(4);
	FreeMem(5);
	FreeMem(6);
	FreeMem(7);
	FreeMem(8);
	FreeMem(1);
	Dump();
	h = AllocMem(10000);
	Dump();
	FreeMem(h);
	Dump();
	return 0;
}

