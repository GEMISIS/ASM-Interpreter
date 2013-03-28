// Include the normal includes.
#include <nds.h>
#include <filesystem.h>

// Include functions for file I/O
#include "fileIO.h"
// Include functions for the interpreter.
#include "interpreter_cmdReader.h"

// A test file for when the file system can't be initialized.
#include "test_asm.h"

// The border to display on the top screen.
const char topScreenBorderText[] = {
	"--------------------------------"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"--------------------------------"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"|                              |"
	"--------------------------------"
};

// Create a top screen and bottom screen console for text.
PrintConsole topScreenBorder, topScreenPC, topScreenMemory, topScreenRegisters, bottomScreen;

void printTopScreenBorder()
{
	// Select the top console.
	consoleSelect(&topScreenBorder);
	// Clear the text.
	consoleClear();

	// Print the border to the screen.
	iprintf("\x1b[%d;%dH%s", 0, 0, topScreenBorderText);

	// Select the bottom console.
	consoleSelect(&bottomScreen);
}

void printfSystemInfo()
{
	// Select the top console's memory section.
	consoleSelect(&topScreenMemory);
	// Clear the console.
	consoleClear();
	// Print the Memory title.
	iprintf("\x1b[%d;%dH%s", 0, 32 / 2 - 4, "Memory");
	// Position the cursor to 0, 1.
	iprintf("\x1b[%d;%dH", 2, 0);
	// Create a foor loop variable.
	int i = 0;
	// Loop through all of the memory.
	for(i = 0;i < MAX_MEM;i += 1)
	{
		// Check if 5 memory cells are displayed
		if(i != 0 && i % 5 == 0)
		{
			// If so, print the next one on a new line.
			iprintf("\n\nM%d:%d\t", i + 1, memory[i]);
		}
		else
		{
			// Otherwise, just print out the memory cell.
			iprintf("M%d:%d\t", i, memory[i]);
		}
	}

	// Select the top console's registers section.
	consoleSelect(&topScreenRegisters);
	// Clear the console.
	consoleClear();
	// Print the Registers title.
	iprintf("\x1b[%d;%dH%s", 0, 32 / 2 - 6, "Registers");
	// Position the cursor to 0, 1.
	iprintf("\x1b[%d;%dH", 2, 0);
	// Loop through all of the registers.
	for(i = 0;i < MAX_REG;i += 1)
	{
		// Check if 5 registers are displayed
		if(i != 0 && i % 5 == 0)
		{
			// If so, print the next one on a new line.
			iprintf("\n\nR%d:%d\t", i + 1, registers[i]);
		}
		else
		{
			// Otherwise, just print out the register.
			iprintf("R%d:%d\t", i, registers[i]);
		}
	}
	// Check if 5 registers are displayed
	if(i != 0 && i % 5 == 0)
	{
		// If so, print the accumulator on a new line.
		iprintf("\n\nAC:%d\t", reg_ac);
	}
	else
	{
		// Otherwise, just print out the accumulator.
		iprintf("AC:%d\t", reg_ac);
	}

	// Select the top console's program counter section.
	consoleSelect(&topScreenPC);
	// Clear the console.
	consoleClear();
	// Print the program counter.
	iprintf("\x1b[%d;%dH%s%d", 0, 0, "PC: ", pc);


	// Select the bottom console.
	consoleSelect(&bottomScreen);
}

int main(void)
{
	// Set the video mode for the top screen.
    videoSetMode(MODE_0_2D);
	// Set the video mode for the bottom screen.
    videoSetModeSub(MODE_0_2D);

	// Allocate video ram for the top screen's text.
    vramSetBankA(VRAM_A_MAIN_BG);
	// Allocate video ram for the bottom screen's text.
    vramSetBankC(VRAM_C_SUB_BG);

	// Initialize the top screen's border text console.
	consoleInit(&topScreenBorder, 0, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	// Initialize the top screen's program counter text console.
	consoleInit(&topScreenPC, 1, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	// Initialize the top screen's memory text console.
	consoleInit(&topScreenMemory, 2, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	// Initialize the top screen's registers text console.
	consoleInit(&topScreenRegisters, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);

	// Set the border console's window position and size.
	consoleSetWindow(&topScreenBorder, 0, 0, 32, 24);
	// Set the program counter console's window position and size.
	consoleSetWindow(&topScreenPC, 1, 1, 10, 2);
	// Set the memory console's window position and size.
	consoleSetWindow(&topScreenMemory, 1, 1, 32 - 2, 24 / 2 - 2);
	// Set the registers console's window position and size.
	consoleSetWindow(&topScreenRegisters, 1, 24 / 2 + 1, 32 - 2, 24 / 2 - 2);

	// Initialize the bottom screen's text console.
	consoleInit(&bottomScreen, 2, BgType_Text4bpp, BgSize_T_256x256, 8, 6, false, true);

	keyboardInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 0, 2, false, true);

	// Select the bottom console.
	consoleSelect(&bottomScreen);

	// Print the border for the top screen.
	printTopScreenBorder();

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
	loadFile("test.asm", &fileBuffer);

	iprintf(".");

	// Check if there is a file buffer.
	if(fileBuffer)
	{
		iprintf(".");
		// If there is, load it as the program.
		loadProgram(false, fileBuffer);
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
			printfSystemInfo();

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
	printfSystemInfo();

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
