#include "interpreter_debugPrinter.h"

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

// Create a top screen screen console for text.
PrintConsole topScreenBorder, topScreenPC, topScreenMemory, topScreenRegisters;

/*
 * Initializes the system for the debug printer.
 */
void initDebugPrinter()
{
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
}

/*
 * Prints the border on the top screen of the Nintendo DS.
 */
void printTopScreenBorder()
{
	// Select the top console.
	consoleSelect(&topScreenBorder);
	// Clear the text.
	consoleClear();

	// Print the border to the screen.
	iprintf("\x1b[%d;%dH%s", 0, 0, topScreenBorderText);
}

/*
 * Prints the memory, registers, and program counter
 * for the interpreter.
 */
void printInterpreterSystemInfo()
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
}

