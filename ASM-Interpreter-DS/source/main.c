// Include the normal includes.
#include <nds.h>
#include <filesystem.h>

// Include functions for the interpreter.
#include "interpreter_dsSystem.h"

// A test file for when the file system can't be initialized.
#include "test_asm.h"

/*
 * Allows the user to type a file on the Nintendo DS.
 */
void CreateProgram(char* fileName);
/*
 * This method actually runs the Nintendo DS program.
 */
void RunProgram(char* fileName, char* stringInput);

/*
 * The main program that is running for the DS.
 * This manages what is going on within the application.
 */
void mainProgramLoop(char* fileName);

/*
 * The status of the application currently.
 */
int applicationStatus = 0;

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

	// Go into an infinite loop now.
	while(1)
	{
		// Loop the main program.
		mainProgramLoop(fileName);
	}

	// Exit out of the program.
	exit(0);
	return 0;
}

void mainProgramLoop(char* fileName)
{
	// Make sure the starting directory is the root directory.
	chdir("/");
	// Prompt the user to run or create a program.
	iprintf("Press A to run a program\nPress B to create a program\nPress Start to exit.");

	while(1)
	{
		// Wait for the vertical blank update.
		swiWaitForVBlank();
		// Scan the keyboard keys.
		scanKeys();

		// Check if the A button is being pressed.
		if(keysDown() & KEY_A)
		{
			applicationStatus = 0;
			// If so, break out of this loop.
			break;
		}
		// Check if the B button is being pressed.
		if(keysDown() & KEY_B)
		{
			applicationStatus = 1;
			// If so, break out of this loop.
			break;
		}
		if(keysDown() & KEY_START)
		{
			applicationStatus = -1;
			// If so, break out of this loop.
			break;
		}
	}

	// Clear the old text from the console.
	consoleClear();
	// Clear the old keys status.
	scanKeys();

	// Switch the possible program states.
	switch(applicationStatus)
	{
	case 0:
		// Run the program at least once.
		RunProgram(fileName, stringInput);
		break;
	case 1:
		// Create a new program.
		CreateProgram(fileName);
		break;
	default:
		// Exit the application.
		exit(0);
		return;
	}
}

/*
 * Allows the user to type a file on the Nintendo DS.
 */
void CreateProgram(char* fileName)
{
	// Select the top console's registers section.
	consoleSelect(&topScreenRegisters);
	// Prompt for a file name.
	iprintf("Please enter a file name\n\n");
	iprintf("Max file name length is\n");
	iprintf("%d characters.\n", MAX_CUST_FILE_ARGS_LEN);
	iprintf("Press the close tab when done\n");
	// Allocate memory for the file name.
	fileName = (char*)malloc((MAX_CUST_FILE_ARGS_LEN + 1) * sizeof(char));
	// Get the name of the file.
	getKeyboardInput(&fileName, MAX_CUST_FILE_ARGS_LEN, false);
	// Check if the file name is valid.
	while(!strcmp(fileName, "\0"))
	{
		// Get the name of the file.
		getKeyboardInput(&fileName, MAX_CUST_FILE_ARGS_LEN, false);
	}
	// Scan for keys to get rid of old inputs.
	scanKeys();
	// Clear the console of old text.
	consoleClear();

	// Select the bottom console.
	consoleSelect(&bottomScreen);

	// Display the current file name.
	iprintf("Current File Name: %s\n", fileName);

	// Create a buffer for the file's data.
	char* fileBuffer = (char*)malloc(512 * sizeof(char));
	// Get the input for the file data.
	getKeyboardInput(&fileBuffer, 512, true);
	// Finally, save the file to the system.
	saveFile(fileName, &fileBuffer);
	// Clear the console.
	consoleClear();

	// Select the top console's registers section.
	consoleSelect(&topScreenRegisters);
	// Then clear the console.
	consoleClear();

	// Select the top console's registers section.
	consoleSelect(&topScreenMemory);
	// Then clear the console.
	consoleClear();

	// Select the bottom console.
	consoleSelect(&bottomScreen);
	// Then clear the console.
	consoleClear();
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

