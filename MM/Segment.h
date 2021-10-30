#pragma once
class Segment
{
public:
	int handle;
	int size;
	int start;
	bool busy;
	Segment()
	{
		handle = 0;
		size = 0;
		start = 0;
		busy = false;
	}
	void del();
};

