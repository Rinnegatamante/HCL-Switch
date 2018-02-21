#ifndef QDA_H
#define QDA_H

#include <unistd.h>

typedef struct {
	unsigned long offset;
	unsigned long size;
	unsigned long bytes;
	unsigned char fileName[256];
} QDAHeader;

QDAHeader headers[29]; //names, offsets, and sizes of each sheet
int initQDA();

#endif