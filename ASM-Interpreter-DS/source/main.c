// Include the normal includes.
#include <nds.h>
#include <filesystem.h>

// Include functions for the interpreter.
#include "interpreter_dsSystem.h"

// A test file for when the file system can't be initialized.
#include "test_asm.h"

/*
 * This method actually runs the Nintendo DS program.
 */
void RunProgram(char* fileName, char* stringInput);

int main(int argc, char* argv[])
{
	// The name of the file to load.
	char* fileName = "test.asm";

	// Check if an argument was passed to the application.
	if(argc > 1)
	{
		// If so, get the length of the argument.
		int nameLength = strlen(argv[1]) < MAX_CUST_FILE_NAME_LEN ? strlen(argv[1]) : MAX_CUST_FILE_NAME_LEN;
		// Then, allocate memory for the name of the file. (Size of the string + 1 for
		// the null terminated character at the end.
		fileName = (char*)malloc((nameLength + 1) * sizeof(char));
		// Copy the argument to the file name string.
		strncpy(fileName, argv[1], nameLength);
		// Set the last character to be null terminated.
		fileName[nameLength] = '\0';
	}

	// Check if another argument was passed to the application.
	if(argc > 2)
	{
		// If so, get the length of the argument.
		int argsLength = strlen(argv[2]) < MAX_CUST_FILE_ARGS_LEN ? strlen(argv[2]) : MAX_CUST_FILE_ARGS_LEN;
		// Then, allocate memory for the name of the file. (Size of the string + 1 for
		// the null terminated character at the end.
		stringInput = (char*)malloc((argsLength + 1) * sizeof(char));
		// Copy the argument to the file name string.
		strncpy(stringInput, argv[2], argsLength);
		// Set the last character to be null terminated.
		stringInput[argsLength] = '\0';
	}

	// Initialize the Nintendo DS system.
	initializeSystem();

	// Now run the program at least once.
	RunProgram(fileName, stringInput);

	// Tell the user their options now that the program has run its course.
	iprintf("Press A to run a new file or Start to quit.");

	// Go into an infinite loop now.
	while(1)
	{
		// Wait for the start key to be pressed and then end the program.
		swiWaitForVBlank();
		// Scan the keyboard keys.
		scanKeys();
		// Check if the start button is being pressed.
		if(keysDown() & KEY_START)
		{
			// If so, break out of this loop.
			break;
		}
		// Check if the A button is being pressed.
		if(keysDown() & KEY_A)
		{
			consoleClear();
			// Then run the program again.
			RunProgram(fileName, stringInput);
			
			// Tell the user their options now that the program has run its course.
			iprintf("Press A to run a new file or Start to quit.");
		}
	}

	// Exit out of the program.
	exit(0);
	return 0;
}

/*
 * This method actually runs the Nintendo DS program.
 */
void RunProgram(char* fileName, char* stringInput)
{
	// Prompt for a file name.
	promptForFile(&fileName);
	// Scan for keys to get rid of old inputs.
	scanKeys();
	// Prompt for arguments for the program.
	promptForFileArgs(&stringInput);

	// Clear the console of old text.
	consoleClear();

	// Print that the program is being intialized.
	iprintf("Loading %s", fileName);

	// Initialize the file buffer.
	char* fileBuffer = NULL;

	// Load a test file to the file buffer.
	loadFile(fileName, &fileBuffer);

	iprintf(".");

	// Check if there is a file buffer.
	if(fileBuffer)
	{
		iprintf(".");
		// If there is, load it as the program.
		loadProgram(false, fileBuffer);
		// Free the file buffer now that we are done using it, and
		// set it to NULL.
		free(fileBuffer);
		fileBuffer = NULL;
		iprintf(".");
	}
	else
	{
		iprintf("Failed, loading built in program..");
		iprintf(".");
		// If there is not a file buffer, load the included test program.
		loadProgram(false, (char*)test_asm);
		iprintf(".");
	}
	// Print that the loading is done.
	iprintf("done!\n");

	// Set the program counter to 0.
	pc = 0;
	// Create a play boolean and set it to false.
	bool play = false;

	// Loop while the program counter is not maxed out.
	while(pc < MAX_COMMANDS)
	{
		// Check if the play variable is false.
		if(!play)
		{
			// Print the information about the system currently.
			printInterpreterSystemInfo();

			// Select the bottom console.
			consoleSelect(&bottomScreen);

			// If so, wait for a veritcal blank and scan for key presses.
			swiWaitForVBlank();
			scanKeys();

			// Check if the right key is being pressed.
			if((keysDown() & KEY_RIGHT))
			{
				// Print out the command properties if so. (pc:cmd:arg0:arg1)
				iprintf("%d:%s: %s, %s\n", pc, commands[pc].cmd, commands[pc].args[0], commands[pc].args[1]);
				// Try to run the command.
				if(runCommand(false, commands[pc].cmd, commands[pc].args) != 0)
				{
					// If the command returned an error, break from the program.
					break;
				}
			}
		}
		else
		{
			// If the program is running, just run the command.
			if(runCommand(false, commands[pc].cmd, commands[pc].args) != 0)
			{
				break;
			}
		}

		// If the start button is pressed, forcibly end the program from running.
		if(keysDown() & KEY_START)
		{
			break;
		}
		// If the B button is pressed, set play to true to run through the
		// whole program.
		if(keysDown() & KEY_B)
		{
			// Set play to one.
			play = true;
		}
	}
	// Print the information about the system currently.
	printInterpreterSystemInfo();

	// Select the bottom console.
	consoleSelect(&bottomScreen);

	// Free the file buffer.
	free(fileBuffer);
	// Then set the file buffer to NULL.
	fileBuffer = NULL;

	// Print that the program finished running.
	printf("Finished running!\n\n");
}

