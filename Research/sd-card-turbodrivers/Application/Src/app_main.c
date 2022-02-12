#include "main.h"
#include "fatfs.h"
#include <stdio.h>

#include "../../Library/lib-tsniimash/test.h"

int app_main(void)
{
	FATFS fileSystem;
	FIL testFile;
	char testBuffer[16];
	snprintf(testBuffer, sizeof(testBuffer), "good sum: %d", super_duper_sum(4, 3, 6));
	UINT testBytes;
	FRESULT res;
	if(f_mount(&fileSystem, SDPath, 1) == FR_OK) {
		uint8_t path[13] = "testfile.txt";
		path[12] = '\0';
		res = f_open(&testFile, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);
		res = f_write(&testFile, (uint8_t*) testBuffer, sizeof(testBuffer), &testBytes);
		res = f_close(&testFile);
	}
	return 0;
}
