#ifndef QDA_H
#define QDA_H

#include <unistd.h>

typedef struct {
	uint32_t offset;
	uint32_t size;
	uint32_t bytes;
	uint8_t fileName[256];
} QDAHeader;

QDAHeader headers[29]; //names, offsets, and sizes of each sheet
int initQDA();

#endif