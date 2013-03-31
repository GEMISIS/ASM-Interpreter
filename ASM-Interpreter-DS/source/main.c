// Include the normal includes.
#include <nds.h>
#include <filesystem.h>

// Include functions for file I/O
#include "fileIO.h"

// Include functions for the interpreter.
#include "interpreter_debugPrinter.h"

// A test file for when the file system can't be initialized.
#include "test_asm.h"

// Set the max length of a file name.
#define MAX_CUST_FILE_NAME_LEN 512

// The maximum length of the arguments for running a file.
#define MAX_CUST_FILE_ARGS_LEN 512

// Create a top screen and bottom screen console for text.
PrintConsole topScreen, bottomScreen;

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

	// Set the video mode for the top screen.
    videoSetMode(MODE_0_2D);
	// Set the video mode for the bottom screen.
    videoSetModeSub(MODE_0_2D);

	// Allocate video ram for the top screen's text.
    vramSetBankA(VRAM_A_MAIN_BG);
	// Allocate video ram for the bottom screen's text.
    vramSetBankC(VRAM_C_SUB_BG);

	// Initialize the debug printer.
	initDebugPrinter();

	// Initialize the top screen's text console.
	consoleInit(&topScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);

	// Initialize the bottom screen's text console.
	consoleInit(&bottomScreen, 2, BgType_Text4bpp, BgSize_T_256x256, 8, 6, false, true);

	// Set the border console's window position and size.
	consoleSetWindow(&topScreen, 1, 1, 32 - 2, 24 / 2 - 2);

	// Set the bottom screen console's window position and size.
	consoleSetWindow(&bottomScreen, 0, 0, 32, 14);

	// Initialize the keyboard.
	keyboardInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 2, false, true);

	// Select the bottom console.
	consoleSelect(&bottomScreen);

	// Print the border for the top screen.
	printTopScreenBorder();

	// Select the bottom console.
	consoleSelect(&bottomScreen);

	// Show the on-screen keyboard.
	keyboardShow();

	// Ask the user if they would like to type a file name
	// and state the commands for doing so.
	iprintf("Would you like to\n");
	iprintf("type in a file name?\n");
	iprintf("A for Yes and B for No\n");

	// Wait for the B button to be pressed.
	while(!(keysDown() & KEY_B))
	{
		// Scan the keypad.
		scanKeys();

		// Check if the A button was pressed.
		if(keysDown() & KEY_A)
		{
			// If so, print out the next bit of info for
			// giving a file name.
			iprintf("Max file name length is\n");
			iprintf("%d characters.\n", MAX_CUST_FILE_NAME_LEN);
			iprintf("Press Enter when done\n");

			// Get the character from the keyboard.
			int character = 0;

			// Allocate memory for the file name again.
			fileName = (char*)malloc((MAX_CUST_FILE_NAME_LEN + 1) * sizeof(char)); 

			// Create a cursor for typing the file name.
			int position = 0;

			// Set the first character to be null terminated
			// so that it prints out properly.
			fileName[position] = '\0';

			// Select the top console's memory section.
			consoleSelect(&topScreen);

			// Wait while the character is not negative or the file name length
			// is greater than MAX_CUST_FILE_NAME_LEN.
			while((character > -2 || character == -20 || character == -18) && position < MAX_CUST_FILE_NAME_LEN)
			{
				// Clear the console.
				consoleClear();

				// Set the position to print at.
				iprintf("\x1b[%d;%dH", 0, 0);

				// Get the next character on the keyboard.
				character = keyboardUpdate();

				// Wait for the newline character specifically.
				if(character == '\n')
				{
					// If newline character was found, break out of the loop.
					break;
				}
				// Check if the character is greater than 0.
				else if(character > 0)
				{
					if(fileName[position] == '\0')
					{
						// Set the last character to be null terminated.
						fileName[position + 1] = '\0';
					}
					// Set the current character to the typed character.
					fileName[position] = character;
					// Check if the character is the backspace character.
					if(character == 8)
					{
						// Set the current letter to be null terminated.
						fileName[position] = '\0';
						// Also, decrement the cursor position by 1.
						position -= 1;
						// And set that letter to be null terminated.
						fileName[position] = '\0';
					}
					else
					{
						// Otherwise, increment the cursor position by 1.
						position += 1;
					}
				}

				// Check for a left key press or left arrow key on
				// the keyboard.
				if(character == -20 || (keysDown() & KEY_LEFT))
				{
					// Check that the position is greater than 0.
					if(position > 0)
					{
						// If so, decrement the cursor position by 1.
						position -= 1;
					}
				}
				// Check for a right key press or right arrow key on
				// the keyboard.
				if(character == -18 || (keysDown() & KEY_RIGHT))
				{
					// Check that the current character is not the null
					// terminated character (aka, the last one).
					if(fileName[position] != '\0')
					{
						// If so, increment the cursor position by 1.
						position += 1;
					}
				}
				// Print the file name.
				iprintf("%s", fileName);

				//iprintf("\x1b[%d;%dH", 0, position);
				//iprintf("|");
				// Wait for the next vertical blank update.
				swiWaitForVBlank();
			}

			// Clear the console for use.
			consoleClear();
			
			// Select the bottom console.
			consoleSelect(&bottomScreen);

			break;
		}
		swiWaitForVBlank();
	}

	// Clear the console of old text.
	consoleClear();

	// Scan again for key input to clear the old input.
	scanKeys();

	// Ask the user if they would like to type a file name
	// and state the commands for doing so.
	iprintf("Would you like to\n");
	iprintf("type in a file args?\n");
	iprintf("A for Yes and B for No\n");

	// Wait for the B button to be pressed.
	while(!(keysDown() & KEY_B))
	{
		// Scan the keypad.
		scanKeys();

		// Check if the A button was pressed.
		if(keysDown() & KEY_A)
		{
			// If so, print out the next bit of info for
			// giving a file name.
			iprintf("Max file name length is\n");
			iprintf("%d characters.\n", MAX_CUST_FILE_ARGS_LEN);
			iprintf("Press the close tab when done\n");

			// Get the character from the keyboard.
			int character = 0;

			// Allocate memory for the file name again.
			stringInput = (char*)malloc((MAX_CUST_FILE_ARGS_LEN + 1) * sizeof(char)); 

			// Create a cursor for typing the file name.
			int position = 0;

			// Set the first character to be null terminated
			// so that it prints out properly.
			stringInput[position] = '\0';

			// Select the top console's memory section.
			consoleSelect(&topScreen);

			// The Y position of the cursor since arguments can have newlines.
			int cursor_Y = 0;

			// Wait while the character is not negative or the file name length
			// is greater than MAX_CUST_FILE_NAME_LEN.
			while((character > -2 || character == -20 || character == -18) && position < MAX_CUST_FILE_ARGS_LEN)
			{
				// Clear the console.
				consoleClear();

				// Set the position to print at.
				iprintf("\x1b[%d;%dH", 0, 0);

				// Get the next character on the keyboard.
				character = keyboardUpdate();

				// Check if the character is greater than 0.
				if(character > 0)
				{
					if(stringInput[position] == '\0')
					{
						// Set the last character to be null terminated.
						stringInput[position + 1] = '\0';
					}
					// Set the current character to the typed character.
					stringInput[position] = character;
					// Check if the character is the backspace character.
					if(character == 8)
					{
						// Set the current letter to be null terminated.
						stringInput[position] = '\0';
						// Also, decrement the cursor position by 1.
						position -= 1;
						// And set that letter to be null terminated.
						stringInput[position] = '\0';
					}
					else
					{
						// Otherwise, increment the cursor position by 1.
						position += 1;
					}

					// Check for a newline character.
					if(character == '\n')
					{
						// Increment the Y position of the cursor.
						cursor_Y += 1;
					}
				}

				// Check for a left key press or left arrow key on
				// the keyboard.
				if(character == -20 || (keysDown() & KEY_LEFT))
				{
					// Check that the position is greater than 0.
					if(position > 0)
					{
						// If so, decrement the cursor position by 1.
						position -= 1;
					}
				}
				// Check for a right key press or right arrow key on
				// the keyboard.
				if(character == -18 || (keysDown() & KEY_RIGHT))
				{
					// Check that the current character is not the null
					// terminated character (aka, the last one).
					if(stringInput[position] != '\0')
					{
						// If so, increment the cursor position by 1.
						position += 1;
					}
				}

				// Print the file name.
				iprintf("%s", stringInput);

				//iprintf("\x1b[%d;%dH", 0, position);
				//iprintf("|");
				// Wait for the next vertical blank update.
				swiWaitForVBlank();
			}

			// Clear the console for use.
			consoleClear();
			
			// Select the bottom console.
			consoleSelect(&bottomScreen);

			break;
		}
		swiWaitForVBlank();
	}

	// Clear the console of old text.
	consoleClear();

	// Print out that the file system is being intialized.
	iprintf("Initializing file I/O.");

	// Initialize the file system.
	if(!fatInitDefault())
	{
		// Print out that the file system did not load here.
		iprintf(".");

		// Attempt to intialize the nitro file system.
		if(nitroFSInit(NULL))
		{
			// Print done if successful.
			iprintf(".done!\n");
		}
		else
		{
			// Print that it failed otherwise.
			iprintf(".failed!\n");
		}
	}
	else
	{
		// Print done if it was successful.
		iprintf("..done!\n");
	}

	// Print that the program is being intialized.
	iprintf("Loading program");

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

	// Print that the program finished running.
	printf("Finished running!\n\n");

	while(1)
	{
		// Wait for the start key to be pressed and then end the program.
		swiWaitForVBlank();
		scanKeys();
		if(keysDown() & KEY_START)
		{
			break;
		}
	}

	// Exit out of the program.
	exit(0);
	return 0;
}
