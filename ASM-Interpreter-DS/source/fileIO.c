#include "fileIO.h"

/*
 * Loads a file to a file buffer and returns the file size.
 * @param fileName The name of the file to load.
 * @param fileBuffer The file buffer to store the file to.
 * @return Returns files size on success, otherwise it returns -1.
 */
unsigned int loadFile(const char* fileName, char** fileBuffer)
{
	// Check that the file buffer is empty.
	if(*fileBuffer != NULL)
	{
		// If it is not, free the file buffer.
		free(*fileBuffer);
		// Then set the file buffer to NULL.
		*fileBuffer = NULL;
	}

	// Create a file, and open a test file.
	FILE* file = fopen(fileName, "rb");

	// Check that the file opened correctly.
	if(file)
	{
		// Seek the end of the file.
		fseek(file, 0, SEEK_END);
		// Set the size to be the position of the end (which is the size of the file).
		unsigned int size = ftell(file);
		// Rewind the file to the beginnning.
		rewind(file);

		// Allocate memory for the buffer.
		*fileBuffer = (char*)calloc(size, sizeof(char));

		// Check if the buffer was setup properly.
		if(*fileBuffer != NULL)
		{
			// Otherwise, read the file into the buffer.
			fread(*fileBuffer, 1, size, file);
		}

		// Close the file.
		fclose(file);

		// Finally, return the size of the file.
		return size;
	}
	// Return -1 if the file was not found.
	return -1;
}


