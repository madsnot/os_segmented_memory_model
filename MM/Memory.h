#pragma once

void Shake(void);
int AllocMem(int sz);
void FreeMem(int handle);
int WriteMem(int handle, int offset, int length, const char *data);
int ReadMem(int handle, int offset, int length, char* data);
void Dump(void);
void Init();