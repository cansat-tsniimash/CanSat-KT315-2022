#include "main.h"
#include "fatfs.h"

int app_main(void)
{
	FATFS fileSystem;
	FIL testFile;
	uint8_t testBuffer[16] = "SD write success";
	UINT testBytes;
	FRESULT res;
	if(f_mount(&fileSystem, SDPath, 1) == FR_OK) {
		uint8_t path[13] = "testfile.txt";
		path[12] = '\0';
		res = f_open(&testFile, (char*)path, FA_WRITE | FA_CREATE_ALWAYS);
		res = f_write(&testFile, testBuffer, 16, &testBytes);
		res = f_close(&testFile);
	}
	return 0;
}
