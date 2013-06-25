#ifndef _INTERPRETER_DSSYSTEM_H_
#define _INTERPRETER_DSSYSTEM_H_

#ifdef __cplusplus
extern "C"
{
#endif

// Include the normal includes.
#include <nds.h>
#include <filesystem.h>

// Include functions for file I/O
#include "fileIO.h"

// Include functions for the interpreter.
#include "interpreter_debugPrinter.h"

// Set the max length of a file name.
#define MAX_CUST_FILE_NAME_LEN 512

// The maximum length of the arguments for running a file.
#define MAX_CUST_FILE_ARGS_LEN 512

// Create a top screen and bottom screen console for text.
PrintConsole topScreen, bottomScreen;

/*
 * Initializes the Nintendo DS system.
 */
void initializeSystem();
/*
 * Reads keyboard input from the touch screen.
 * @param input The input to store the text in.
 * @param maxInputLength The maximum number of characters that the input can handle.
 * @param newlineCharacterAllowed True if the newline character can be used, false otherwise if it should
 * end the allowing of text input.
 */
void getKeyboardInput(char** input, int maxInputLength, bool newlineCharacterAllowed);
/*
 * Prompt the user to select a file from the file system.
 * @param fileName The file name to store the selected file name in.
 */
void promptForFile(char** fileName);
/*
 * Prompt the user to input arguments for the file.
 * @param arguments The array of characters to store the argument text in
 */
void promptForFileArgs(char** arguments);

#ifdef __cplusplus
}
#endif

#endif
