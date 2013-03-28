#ifndef _FILEIO_H_
#define _FILEIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include <fat.h>
#include <filesystem.h>
#include <stdio.h>

/*
 * Loads a file to a file buffer and returns the file size.
 * @param fileName The name of the file to load.
 * @param fileBuffer The file buffer to store the file to.
 * @return Returns files size on success, otherwise it returns -1.
 */
unsigned int loadFile(const char* fileName, char** fileBuffer);

#ifdef __cplusplus
}
#endif

#endif
