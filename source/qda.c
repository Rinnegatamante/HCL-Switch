#include "qda.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _PSP2
extern uint8_t use_uma0;
#endif

//Load headers for each image
//Returns: 0 = file not found | 1 = success | 2 = Invalid file
int initQDA()
{
	int result = 0;
	FILE* f;
	
	char fullPath[80];
	strcpy(fullPath, "");
	#ifdef _3DS
		strcat(fullPath, "romfs:/");
	#endif
	#ifdef _PSP2
		sprintf(fullPath, "%s:data/HCL/bmp.qda", use_uma0 ? "uma0" : "ux0");
	#else
	strcat(fullPath, "./bmp.qda");
	#endif
	if ( (f = fopen(fullPath, "rb")) ) {
		result = 1;
		
		//Read header data into memory
		int allHeadersSize = 0x1F5C;
		unsigned char* QDAFile = (unsigned char*)malloc(allHeadersSize);
		fread(QDAFile, allHeadersSize, 1, f);
		
		//Check if QDA file is valid
		{
			if (QDAFile[4] == 0x51 && QDAFile[5] == 0x44 && QDAFile[6] == 0x41 && QDAFile[7] == 0x30) {
				//Load headers separately
				{
					int numofsheets = 29;
					int headerSize = 0x10C;
				
					int i;
					for (i = 0; i < numofsheets; i++) {
						//memcpy(&headers[i], &QDAFile[0x100 + (i * headerSize)], sizeof(QDAHeader));
						int offset = 256 + (i * headerSize);						
						memcpy(&headers[i].offset, &QDAFile[offset], 4);
						memcpy(&headers[i].size, &QDAFile[offset + 4], 4);
						memcpy(&headers[i].bytes, &QDAFile[offset + 8], 4);
						memcpy(&headers[i].fileName, &QDAFile[offset + 12], 0x100);
					}
				}
			}else{
				result = 2;
			}
		}
		
		//Cleanup
		free(QDAFile);
	}
	
	fclose(f);
	
	return result;
}