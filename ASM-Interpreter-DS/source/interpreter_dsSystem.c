#include "interpreter_dsSystem.h"

#include <unistd.h>
#include <dirent.h>

#include "neocompoLogo.h"
#include "devLogo.h"

/*
 * Displays the logos for NeoCompo 2013.
 */
void displayCompoLogo()
{
	// Set the video mode for the top and bottom screens to support extended background layers.
    videoSetMode(MODE_5_2D);
    videoSetModeSub(MODE_5_2D);

	// Initialize the main background.
	int backgroundMain = bgInit(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);
	// Initialize the sub background.
	int backgroundSub = bgInitSub(3, BgType_Bmp8, BgSize_B8_256x256, 0,0);

	// Copy the main background's data.
	dmaCopy(neocompoLogoBitmap, bgGetGfxPtr(backgroundMain), 65536);
	// Copy the sub background's data.
	dmaCopy(devLogoBitmap, bgGetGfxPtr(backgroundSub), 65536);
	// Copy the main background's palette.
	dmaCopy(neocompoLogoPal, BG_PALETTE, 512);
	// Copy the sub background's palette.
	dmaCopy(devLogoPal, BG_PALETTE_SUB, 512);

	// Fade the brightness back in.
	for(int i = -16;i < 1;i += 1)
	{
		setBrightness(3, i);
		swiWaitForVBlank();
	}

	// Wait for 5 seconds on the logo.
	for(int i = 0;i < 60 * 5;i += 1)
	{
		swiWaitForVBlank();
		// Allow the user to skip the logos after at least 2 seconds.
		if(i > 60 * 1)
		{
			scanKeys();
			if(keysDown() & KEY_A)
			{
				break;
			}
		}
	}

	// Fade back out.
	for(int i = 0;i > -17;i -= 1)
	{
		setBrightness(3, i);
		swiWaitForVBlank();
	}

	// Zero out the data from the top and bottom screen backgrounds.
	memset(bgGetGfxPtr(backgroundMain), 0, 65536);
	memset(bgGetGfxPtr(backgroundSub), 0, 65536);
	// Zero out the palette slots as well.
	memset(BG_PALETTE, 0, 512);
	memset(BG_PALETTE_SUB, 0, 512);

	// Set the video mode for the top and bottom screens back to normal.
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
}

/*
 * Initializes the Nintendo DS system.
 */
void initializeSystem()
{
	// Set the screen brightness to be completely dark.
	setBrightness(3, -16);

	// Displays the logo for the Neocompo 2013.
	displayCompoLogo();

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

	// Clear the console of old text.
	consoleClear();

	// Scan again for key input to clear the old input.
	scanKeys();

	// Fade the brightness back in.
	for(int i = -16;i < 1;i += 1)
	{
		setBrightness(3, i);
		swiWaitForVBlank();
	}

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
			iprintf(".done!(NITRO)\n");
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
		iprintf("..done!(FAT)\n");
	}
}

/*
 * Reads keyboard input from the touch screen.
 * @param input The input to store the text in.
 * @param maxInputLength The maximum number of characters that the input can handle.
 * @param newlineCharacterAllowed True if the newline character can be used, false otherwise if it should
 * end the allowing of text input.
 */
void getKeyboardInput(char** input, int maxInputLength, bool newlineCharacterAllowed)
{
	// Create a cursor for typing the file name.
	int position = 0;

	// Check that the input is null.
	if(*input != NULL)
	{
		// If it is not, free the memory.
		free(*input);
		// Then set it to null.
		*input = NULL;
	}
	// Allocate memory for the file name again.
	*input = (char*)malloc((maxInputLength + 1) * sizeof(char)); 

	// Set the first character to be null terminated
	// so that it prints out properly.
	memset(*input, '\0', maxInputLength + 1);

	// Select the top console's memory section.
	consoleSelect(&topScreen);

	// Get the character from the keyboard.
	int character = 0;

	// The blink status for the cursor.
	bool blinkStatus = true;
	// Store the character that the cursor is over.
	char cursorChar = '\0';
	// A counter for the blink status.
	int blinkCounter = 0;
	// The offset for where to start drawing the text from.
	int offset = 0;
	// The total amount of lines in the text.
	int lines = 0;

	// Wait while the character is not negative or the file name length
	// is greater than MAX_CUST_FILE_NAME_LEN.
	while((character > -2 || character ==  DVK_LEFT || character == DVK_RIGHT || character == DVK_SHIFT) && position < MAX_CUST_FILE_NAME_LEN)
	{
		// Scan for key pressed.
		scanKeys();

		// Wait for the next vertical blank update.
		swiWaitForVBlank();

		if(blinkCounter > 15)
		{
			blinkStatus = !blinkStatus;
			blinkCounter = -1;
		}
		blinkCounter += 1;
		// Clear the console.
		consoleClear();

		// Set the position to print at.
		iprintf("\x1b[%d;%dH", 0, 0);

		// Get the next character on the keyboard.
		character = keyboardUpdate();

		// Check if the up key is being pressed.
		if(keysDown() & KEY_UP)
		{
			// Check that the offset is greater than 0.
			if(offset > 0)
			{
				// If so, decrement the offset.
				offset -= 1;
			}
		}
		// Check if the down key is being pressed.
		if(keysDown() & KEY_DOWN)
		{
			// Check that the offset is less than the number of lines.
			if(offset < lines - 9)
			{
				// If so, increment the offset.
				offset += 1;
			}
		}

		// Check if the blink status is false and the cursor should be visible.
		if(!blinkStatus)
		{
			// Store the cursor's character.
			cursorChar = (*input)[position];
			// Check that the cursor character is not a newline character.
			if(cursorChar != '\n')
			{
				// Then set the character at that position to the cursor.
				(*input)[position] = '|';
			}

			// Create a temporary character iterator.
			int i = 0;
			// Count how many lines have been displayed.
			int lineCount = 0;
			// Loop while the iterator still has characters to display.
			while(i < strlen(*input))
			{
				// Check if the character is a new line character.
				if((*input)[i] == '\n')
				{
					// If so, increment the line count.
					lineCount += 1;
				}

				// Check if the line count is greater than 9.
				if(lineCount - offset > 9)
				{
					// If so, break out of the loop.
					break;
				}
				else if(lineCount >= offset)
				{
					// Print the current character.
					iprintf("%c", (*input)[i]);
				}

				// Increment the character iterator.
				i += 1;
			}

			// Finally, set the character back to normal.
			(*input)[position] = cursorChar;
		}
		else
		{
			// Create a temporary character iterator.
			int i = 0;
			// Count how many lines have been displayed.
			int lineCount = 0;
			// Loop while the iterator still has characters to display.
			while(i < strlen(*input))
			{
				// Check if the character is a new line character.
				if((*input)[i] == '\n')
				{
					// If so, increment the line count.
					lineCount += 1;
				}

				// Check if the line count is greater than 9.
				if(lineCount - offset > 9)
				{
					// If so, break out of the loop.
					break;
				}
				else if(lineCount >= offset)
				{
					// Print the current character.
					iprintf("%c", (*input)[i]);
				}

				// Increment the character iterator.
				i += 1;
			}
		}

		// Check if the maximum amount of characters was reached and if
		// the next key is a backspace keystroke.
		if(strlen(*input) >= maxInputLength && character != 8)
		{
			// If not, continue through the loop.
			continue;
		}

		// Wait for the newline character specifically.
		if(!newlineCharacterAllowed && character == '\n')
		{
			// If newline character was found, break out of the loop.
			break;
		}
		// Check if the character is greater than 0.
		else if(character > 0)
		{
			// Check if the typed charactrer is a newline character.
			if(character == '\n')
			{
				// Increment the amount of lines.
				lines += 1;
				// Check if the amount of lines is greater than 9.
				if(lines > 9)
				{
					// If so, increment the offset.
					offset += 1;
				}
			}
			// Check if the current character is a null terminated character.
			if((*input)[position] == '\0')
			{
				// Set the last character to be null terminated.
				(*input)[position + 1] = '\0';
			}
			// Set the current character to the typed character.
			(*input)[position] = character;
			// Check if the character is the backspace character.
			if(character == 8 && position > 0)
			{
				// Check if the current character is a newline character.
				if((*input)[position] == '\n')
				{
					// If so, decrement the offset.
					offset -= 1;
					// Also decrement the number of lines.
					lines += 1;
				}
				// Set the current letter to be null terminated.
				(*input)[position] = '\0';
				// Also, decrement the cursor position by 1.
				position -= 1;
				// And set that letter to be null terminated.
				(*input)[position] = '\0';
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
			if((*input)[position] != '\0')
			{
				// If so, increment the cursor position by 1.
				position += 1;
			}
		}
	}
}

/*
 * A structure for directory entries.  Contains
 * the name of the entry, and it's type (file or directory).
 */
typedef struct
{
	// The name of the entry.
	char name[NAME_MAX];
	// The type of entry.
	unsigned char type;
}directoryEntry;

/*
 *  Prints a list of files onto the screen starting with the chosen
 *  offset.
 * @param offset The offset to start printing the files from.
 * @param fileNames The list of files to print.
 */
void printFiles(int offset, directoryEntry* fileNames)
{
	// Loop through the possible entries.
	for(int i = offset;i < offset + 13;i += 1)
	{
		// Set the text position to the correct spot.
		printf("\033[%d;%dH", i - offset, 1);
		// Print the entry's name.
		printf("%s\n", fileNames[i].name);
	}
}

u32 listFiles(const char* startDirectory, directoryEntry** fileNames)
{
	if(*fileNames != NULL)
	{
		free(*fileNames);
		*fileNames = NULL;
	}
	u32 filesCount = 0;
	struct dirent* dirEntry;

	DIR* topDirectory = opendir(startDirectory);
	if(topDirectory != NULL)
	{
		while((dirEntry = readdir(topDirectory)) != NULL)
		{
			filesCount += 1;
		}
		rewinddir(topDirectory);
		*fileNames = (directoryEntry*)malloc(filesCount * sizeof(directoryEntry));
		int i = 0;
		while((dirEntry = readdir(topDirectory)) != NULL)
		{
			strncpy((*fileNames)[i].name, dirEntry->d_name, NAME_MAX);
			(*fileNames)[i].type = dirEntry->d_type;
			i += 1;
		}
		closedir(topDirectory);
	}

	return filesCount;
}

void openFileBrowser(char** fileName)
{
	directoryEntry* fileNames = NULL;
	u32 filesCount = listFiles("/", &fileNames);

	int line = 0;
	int offset = 0;

	while(!(keysDown() & KEY_START))
	{
		scanKeys();
		swiWaitForVBlank();
		consoleClear();
		printFiles(offset, fileNames);
		printf("\033[%d;%dH-", line - offset, 0);

		if(keysDown() & KEY_A)
		{
			if(fileNames[line].type == DT_DIR)
			{
				char* dir = (char*)malloc((strlen(fileNames[line].name) + 1) * sizeof(char));
				strncpy(dir, fileNames[line].name, strlen(fileNames[line].name));
				dir[strlen(fileNames[line].name)] = '\0';

				line = 0;
				offset = 0;

				for(int i = 0;i < filesCount;i += 1)
				{
					memset(fileNames[i].name, '\0', strlen(fileNames[i].name));
				}

				filesCount = listFiles(dir, &fileNames);
				chdir(dir);

				free(dir);
				dir = NULL;
			}
			else
			{
				*fileName = (char*)malloc((NAME_MAX + 1) * sizeof(char)); 
				strncpy(*fileName, fileNames[line].name, NAME_MAX);
				break;
			}
		}
		if(keysDown() & KEY_B)
		{
			break;
		}
		if(keysDown() & KEY_UP)
		{
			if(line > 0)
			{
				printf("\033[%d;%dH ", line - offset, 0);
				line -= 1;

				if(line < offset)
				{
					offset -= 1;
				}
			}
			else
			{
				printf("\033[%d;%dH ", line - offset, 0);
				line = filesCount - 1;

				if(filesCount > 12)
				{
					offset = filesCount - 13;
				}
				else
				{
					offset = 0;
				}
			}
		}
		if(keysDown() & KEY_DOWN)
		{
			if(line < filesCount - 1)
			{
				printf("\033[%d;%dH ", line - offset, 0);
				line += 1;

				if(line - 12 > offset)
				{
					offset += 1;
				}
			}
			else
			{
				printf("\033[%d;%dH ", line - offset, 0);
				line = 0;

				offset = 0;
			}
		}
	}
}

/*
 *  Gets a file name that is typed in.
 * @param fileName The file name to store the selected file name in.
 */
void getTypedFile(char** fileName)
{
	// Select the top console's registers section.
	consoleSelect(&topScreenRegisters);

	// If so, print out the next bit of info for
	// giving a file name.
	iprintf("Max file name length is\n");
	iprintf("%d characters.\n", MAX_CUST_FILE_ARGS_LEN);
	iprintf("Press the close tab when done\n");

	// Allocate memory for the file name again.
	*fileName = (char*)malloc((MAX_CUST_FILE_NAME_LEN + 1) * sizeof(char)); 

	// Set the first character to be null terminated
	// so that it prints out properly.
	(*fileName)[0] = '\0';

	// Get keyboard input here.
	getKeyboardInput(fileName, MAX_CUST_FILE_NAME_LEN, false);

	// Select the top console's registers section.
	consoleSelect(&topScreenRegisters);
	// Then clear the console.
	consoleClear();

	// Select the bottom console.
	consoleSelect(&bottomScreen);
}

/*
 * Prompt the user to select a file from the file system.
 * @param fileName The file name to store the selected file name in.
 */
void promptForFile(char** fileName)
{
	// Ask the user if they would like to type a file name
	// and state the commands for doing so.
	iprintf("Would you like to\n");
	iprintf("type in a file name\n");
	iprintf("or browser for a file?\n\n");
	iprintf("Up to browse, down to type\n");
	iprintf("and B for neither.\n");

	// Wait for the B button to be pressed.
	while(!(keysDown() & KEY_B) && !(keysDown() & KEY_UP) && !(keysDown() & KEY_DOWN))
	{
		// Scan the keypad.
		scanKeys();

		// Check if the up button was pressed.
		if(keysDown() & KEY_UP)
		{
			// If so, browse for a file.
			openFileBrowser(fileName);
			break;
		}
		// Check if the down button was pressed.
		if(keysDown() & KEY_DOWN)
		{
			// If so, get a file that's typed in.
			getTypedFile(fileName);
			break;
		}
		swiWaitForVBlank();
	}
	// Finally, clear the console.
	consoleClear();
}

/*
 * Prompt the user to input arguments for the file.
 * @param arguments The array of characters to store the argument text in
 */
void promptForFileArgs(char** arguments)
{
	// Ask the user if they would like to type a file name
	// and state the commands for doing so.
	iprintf("Would you like to\n");
	iprintf("type in file args?\n");
	iprintf("A for Yes and B for No\n");

	// Wait for the B button to be pressed.
	while(!(keysDown() & KEY_B) && !(keysDown() & KEY_A))
	{
		// Scan the keypad.
		scanKeys();

		// Check if the A button was pressed.
		if(keysDown() & KEY_A)
		{
			// Select the top console's registers section.
			consoleSelect(&topScreenRegisters);
			// If so, print out the next bit of info for
			// giving a file name.
			iprintf("Max file arguments length is\n");
			iprintf("%d characters.\n", MAX_CUST_FILE_ARGS_LEN);
			iprintf("Press the close tab when done\n");

			// Allocate memory for the file name again.
			*arguments = (char*)malloc((MAX_CUST_FILE_ARGS_LEN + 1) * sizeof(char)); 

			// Set the first character to be null terminated
			// so that it prints out properly.
			(*arguments)[0] = '\0';

			// Get keyboard input here.
			getKeyboardInput(arguments, MAX_CUST_FILE_ARGS_LEN, true);

			// Select the top console's registers section.
			consoleSelect(&topScreenRegisters);
			// Then clear the console.
			consoleClear();

			// Select the bottom console.
			consoleSelect(&bottomScreen);
		}
		swiWaitForVBlank();
	}
}




