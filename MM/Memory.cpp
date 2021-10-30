#include <iostream>
#include "Segment.h"
#include "Memory.h"

using namespace std;
#define NUM_OF_BLOCS 30
#define MEM_SIZE 10000

int Handle;
char mem[MEM_SIZE];
Segment bloc[NUM_OF_BLOCS];

void Init()
{
	int i;
	if (bloc[0].handle == 0)
	{
		bloc[0].handle = 1;
		bloc[0].size = MEM_SIZE;
		Handle = 1;
	}
}

int AllocMem(int sz)
{
	#define NOT_FOUND -1
	int i, k = NOT_FOUND, freeSpace = 0, j = NOT_FOUND, minSize = MEM_SIZE;
	if (sz <= 0)
		return 0;
	for (i = 0; i < NUM_OF_BLOCS; i++)
	{
		if (!bloc[i].busy)
		{
			if (bloc[i].handle == 0)
				j = i;
			if (bloc[i].size >= sz && bloc[i].size <= minSize)
			{
				minSize = bloc[i].size;
				k = i;
			}
			freeSpace += bloc[i].size;
		}
	}
	if (freeSpace < sz || j == NOT_FOUND)
		return 0;
	if (k == NOT_FOUND)
	{
		Shake();
		k = 0;
	}
	bloc[k].size -= sz;
	if (bloc[k].size == 0)
		j = k;
	else
	{
		Handle++;
		bloc[j].handle = Handle;
	}
	bloc[j].start = bloc[k].start +  bloc[k].size;
	bloc[j].size = sz;
	bloc[j].busy = true;
	return bloc[j].handle;
}

int WriteMem(int handle, int offset, int length, const char* data)
{
	int i = 0, j, k;
	while (data[i])
		i++;
	if (length > i)
		length = i;
	for (i = 0; i < NUM_OF_BLOCS; i++)
		if (bloc[i].busy && bloc[i].handle == handle && length <= bloc[i].size - offset)
		{
			k = bloc[i].start + offset;
			for (j = 0; j < length; j++)
			{
				mem[k] = data[j];
				k++;
			} 
			return 0;
		}
		return 1;
}

int ReadMem(int handle, int offset, int length, char* data)
{
	int i, j, k;
	for (i = 0; i < NUM_OF_BLOCS; i++)
		if (bloc[i].busy && bloc[i].handle == handle && length <= bloc[i].size - offset)
		{
			j = 0;
			for (k = bloc[i].start + offset; k < bloc[i].start + offset + length; k++)
			{
				data[j] = mem[k];
				j++;
			}
			data[j] = '\0';
			return 0;
		}
	return 1;
}

void FreeMem(int handle)
{
	int i, k;
	for (i = 0; i < NUM_OF_BLOCS; i++)
		if (bloc[i].busy && bloc[i].handle == handle)
		{
			for (k = bloc[i].start; k < bloc[i].start + bloc[i].size; k++)
				mem[k] = ' ';
			bloc[i].busy = false;
		}	
}

void Shake()
{
	int size, start, handle, i, j, freeSpace = 0, usedSpace, oldStart;
	bool flag, busy;
	do
	{
		flag = true;
		for (i = 0; i < NUM_OF_BLOCS-1; i++)
		{
			if (bloc[i+1].handle > 0 && !bloc[i+1].busy)
			{
				freeSpace += bloc[i+1].size;
				bloc[i+1].del();
			}
			if (bloc[i].busy > bloc[i + 1].busy || (bloc[i].busy == bloc[i + 1].busy && bloc[i].start > bloc[i + 1].start))
			{
				busy = bloc[i].busy;
				size = bloc[i].size;
				start = bloc[i].start;
				handle = bloc[i].handle;
				bloc[i].busy = bloc[i + 1].busy;
				bloc[i].size = bloc[i + 1].size;
				bloc[i].start = bloc[i + 1].start;
				bloc[i].handle = bloc[i + 1].handle;
				bloc[i + 1].busy = busy;
				bloc[i + 1].size = size;
				bloc[i + 1].start = start;
				bloc[i + 1].handle = handle;
				flag = false;
			}
		}
	} while (!flag);
	bloc[0].size += freeSpace;
	if (bloc[0].handle == 0 && bloc[0].size > 0)
	{
		Handle++;
		bloc[0].handle = Handle;
	}
	for (i = NUM_OF_BLOCS-1; i >= 0; i--)
	{
		if (bloc[i].busy)
		{
			oldStart = bloc[i].start;
			usedSpace = 0;
			j = i-1;
			while (bloc[j].busy)
			{
				usedSpace += bloc[j].size;
				j--;
			}
			bloc[i].start = bloc[0].size + usedSpace;
			start = bloc[i].start + bloc[i].size - 1;
			if (bloc[i].start != oldStart)
				for (j = oldStart + bloc[i].size - 1; j >= oldStart; j--)
				{	
					mem[start] = mem[j];
					start--;
				}
		}
	}
}

void Dump(void)
{
	int i, j, bytes;
	for (i = 0; i < NUM_OF_BLOCS; i++)
	{
		bytes = 10;
		if (bloc[i].handle != 0)
		{
			cout << "H:" << bloc[i].handle << " ";
			cout << "B:" << bloc[i].start << " ";
			cout << "S:" << bloc[i].size << " ";
			if (!bloc[i].busy)
				cout << "No ";
			else
			{
				cout << "Yes ";
				if (bloc[i].size < bytes)
					bytes = bloc[i].size;
				for (j = bloc[i].start; j < bloc[i].start + bytes; j++)
					cout << mem[j];
			}
			cout << endl;
		}
	}
	cout << endl;
}