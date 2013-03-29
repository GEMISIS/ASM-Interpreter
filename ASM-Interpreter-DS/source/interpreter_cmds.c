#include "interpreter_cmds.h"

// The program counter.
unsigned int pc = 0;
// The accumulator register.
s32 reg_ac = 0;
// The array of registers.
s32 registers[MAX_REG];
// The array of memory locations.
s32 memory[MAX_MEM];

// The list of commands to run.
command_t commands[MAX_COMMANDS];

/*
 * Add the values in registers r1 and r2 and store the value in the accumulator.
 * @param r1 The first register to add. (-1 to use the accumulator)
 * @param r2 The second register to add. (-1 to use the accumulator)
 */
void add(int r1, int r2)
{
	// If both registers are greater than -1
	if(r1 > -1 && r2 > -1)
	{
		// Then add those registers.
		reg_ac = registers[r1] + registers[r2];
	}
	// If the first register is less than 0
	if(r1 < 0 && r2 > -1)
	{
		// Then the first register is the accumulator.
		reg_ac = reg_ac + registers[r2];
	}
	// If the second register is less than 0
	if(r1 > -1 && r2 < 0)
	{
		// Then the second register is the accumulator.
		reg_ac = registers[r1] + reg_ac;
	}
	// If both registers are less than 0
	if(r1 < 0 && r2 < 0)
	{
		// Then both registers are the accumulator.
		reg_ac = reg_ac + reg_ac;
	}
}
/*
 * Subtract the values in registers r1 and r2 from each other and store the value in the accumulator.
 * @param r1 The register to subtract from. (-1 to use the accumulator)
 * @param r2 The register to subtract with. (-1 to use the accumulator)
 */
void sub(int r1, int r2)
{
	// If both registers are greater than -1
	if(r1 > -1 && r2 > -1)
	{
		// Then subtract those registers.
		reg_ac = registers[r1] - registers[r2];
	}
	// If the first register is less than 0
	if(r1 < 0 && r2 > -1)
	{
		// Then the first register is the accumulator.
		reg_ac = reg_ac - registers[r2];
	}
	// If the second register is less than 0
	if(r1 > -1 && r2 < 0)
	{
		// Then the second register is the accumulator.
		reg_ac = registers[r1] - reg_ac;
	}
	// If both registers are less than 0
	if(r1 < 0 && r2 < 0)
	{
		// Then both registers are the accumulator.
		reg_ac = reg_ac - reg_ac;
	}
}
/*
 * Multiply the values in registers r1 and r2 and store the value in the accumulator.
 * @param r1 The first register to multiply. (-1 to use the accumulator)
 * @param r2 The second register to multiply. (-1 to use the accumulator)
 */
void mul(int r1, int r2)
{
	// If both registers are greater than -1
	if(r1 > -1 && r2 > -1)
	{
		// Then multiply those registers.
		reg_ac = registers[r1] * registers[r2];
	}
	// If the first register is less than 0
	if(r1 < 0 && r2 > -1)
	{
		// Then the first register is the accumulator.
		reg_ac = reg_ac * registers[r2];
	}
	// If the second register is less than 0
	if(r1 > -1 && r2 < 0)
	{
		// Then the second register is the accumulator.
		reg_ac = registers[r1] * reg_ac;
	}
	// If both registers are less than 0
	if(r1 < 0 && r2 < 0)
	{
		// Then both registers are the accumulator.
		reg_ac = reg_ac * reg_ac;
	}
}
/*
 * Divide the values in registers r1 and r2 and store the value in the accumulator.
 * @param r1 The number to divide. (-1 to use the accumulator)
 * @param r2 The number to divide by. (-1 to use the accumulator)
 */
void divide(int r1, int r2)
{
	// If both registers are greater than -1
	if(r1 > -1 && r2 > -1)
	{
		// Then divide the registers.
		reg_ac = registers[r1] / registers[r2];
	}
	// If the first register is less than 0
	if(r1 < 0 && r2 > -1)
	{
		// Then the first register is the accumulator.
		reg_ac = reg_ac / registers[r2];
	}
	// If the second register is less than 0
	if(r1 > -1 && r2 < 0)
	{
		// Than the second register is the accumulator.
		reg_ac = registers[r1] / reg_ac;
	}
	// If both registers are less than 0
	if(r1 < 0 && r2 < 0)
	{
		// Then both registers are the accumulator.
		reg_ac = reg_ac / reg_ac;
	}
}
/*
 * Perform the modulous function with the two registers and store the value 
 * @param r1 The number to use the modulous on. (-1 to use the accumulator)
 * @param r2 The modulous value to use. (-1 to use the accumulator)
 */
void mod(int r1, int r2)
{
	// If both registers are greater than -1
	if(r1 > -1 && r2 > -1)
	{
		// Then mod the two registers.
		reg_ac = registers[r1] % registers[r2];
	}
	// If the first register is less than 0
	if(r1 < 0 && r2 > -1)
	{
		// Then the first register is the accumulator.
		reg_ac = reg_ac % registers[r2];
	}
	// If the second register is less than 0
	if(r1 > -1 && r2 < 0)
	{
		// Then the second register is the accumulator.
		reg_ac = registers[r1] % reg_ac;
	}
	// If both registers are less than 0
	if(r1 < 0 && r2 < 0)
	{
		// Then both registers are the accumulator.
		reg_ac = reg_ac % reg_ac;
	}
}
/*
 * Store the value in the chosen register to a memory location.
 * @param m1 The memory location to store the value in.
 * @param r1 The register to get the value from. (-1 to use the accumulator)
 */
void store(int m1, int r1)
{
	// If the register is less than 0, then store the accumulator.
	if(r1 < 0)
	{
		memory[m1] = reg_ac;
	}
	// Otherwise, store the register.
	else
	{
		memory[m1] = registers[r1];
	}
}
/*
 * Loads the value in a chosen memory location to a register.
 * @param r1 The register to store the value to. Note that you cannot overwrite
 * the accumulator, so -1 is not a valid value.
 * @param m1 The memory location to get the value from.
 */
void load(int r1, int m1)
{
	// Check if the register is less than 0.
	if(r1 < 0)
	{
		// If so, display an error.
		iprintf("Can't load to AC register!\n");
	}
	else
	{
		// Otherwise, set the register to the memory value.
		registers[r1] = memory[m1];
	}
}
/*
 * Copies the value in the first register to the second register.
 * @param r1 The register to load the value from. (-1 to use the accumulator)
 * @param r2 The register to store the value to. Note that you cannot overwrite
 * the accumulator, so -1 is not a valid value.
 */
void copy(int r1, int r2)
{
	// Check if the second register is less than 0.
	if(r2 < 0)
	{
		// If so, display an error.
		iprintf("Can't copy to AC register!\n");
	}
	// Otherwise check if the first register is less than 0.
	else if(r1 < 0)
	{
		// If so, store the accumulator in the second register.
		registers[r2] = reg_ac;
	}
	else
	{
		// Otherwise, store the first register's value in the second register.
		registers[r2] = registers[r1];
	}
}
/*
 * Jumps to a specific position in the program. (IE: Sets the program counter)
 * @param label The position to jump to.
 */
void jmp(unsigned int label)
{
	pc = label;
}
/*
 * Sets a register to a specific value.
 * @param r1 The register to set.
 * @param value The value to set the register to.
 */
void set(int r1, s32 value)
{
	registers[r1] = value;
}
/*
 * Jumps if the register does not have the same value as the accumulator.
 * @param r1 The register to check the value of.
 * @param label The label to jump to.
 */
void jne(int r1, unsigned int label)
{
	// Check if the register is less than 0.
	if(r1 < 0)
	{
		// If so, display an error message.
		iprintf("Can't check AC register with AC registers!\n");
	}
	// Check if the register value is not equal to the accumulator.
	else if(registers[r1] != reg_ac)
	{
		// If so, set the program counter to the label.
		pc = label;
	}
}
/*
 * Jumps if the register has the same value as the accumulator.
 * @param r1 The register to check the value of.
 * @param label The label to jump to.
 */
void jeq(int r1, unsigned int label)
{
	// Check if the register is less than 0.
	if(r1 < 0)
	{
		// If so, display an error message.
		iprintf("Can't check AC register with AC registers!\n");
	}
	// Check if the register value is equal to the accumulator.
	else if(registers[r1] == reg_ac)
	{
		// If so, set the program counter to the label.
		pc = label;
	}
}
/*
 * Compares two registers and stores the comparison value in the accumulator.
 * -1 if the first register is less than the second, 1 if the first register
 * is greater than the second, and 0 if they are equal.
 * @param r1 The first register to check the value of.
 * @param r2 The second register to check the value of.
 */
void cmp(int r1, int r2)
{
	reg_ac = (registers[r1] > registers[r2]) ? 1 : (registers[r1] == registers[r2]) ? 0 : -1;
}
/*
 * Prints the value of the register as an integer.
 * @param r1 The register to print the value of. (-1 to use the accumulator)
 */
void print(int r1)
{
	// Check if the register is less than 0.
	if(r1 < 0)
	{
		// If so, print the accumulator.
		iprintf("%d", reg_ac);
	}
	else
	{
		// Otherwise, print the register.
		iprintf("%d", registers[r1]);
	}
}
/*
 * Prints the value of the register as an character.
 * @param r1 The register to print the value of. (-1 to use the accumulator)
 */
void printc(int r1)
{
	// Check if the register is less than 0.
	if(r1 < 0)
	{
		// If so, print the accumulator.
		iprintf("%c", reg_ac);
	}
	else
	{
		// Otherwise, print the register.
		iprintf("%c", registers[r1]);
	}
}
/*
 * Gets a character from the keyboard.  On the Nintendo DS, this is done
 * via a keyboard that appears on the bottom screen.
 * @param r1 The register to store the value in. (-1 to use the accumulator)
 */
void getcFunc(int r1)
{
	// The character to return.
	int character = -1;

	// Show the on-screen keyboard.
	keyboardShow();
	// Check whether a key has been pressed.
	while(character == -1)
	{
		// Get the keyboard character.
		character = keyboardUpdate();
		// Wait for a vertical refresh.
		swiWaitForVBlank();
	}
	// Check if a key that is not a letter or number was pressed.
	if(character < 0)
	{
		// If so, set the character to -1.
		character = -1;
	}
	// Hide the keyboard.
	keyboardHide();
	// Print the character that was typed.
	iprintf("%c\n", character);
	// Set the register to the character.
	registers[r1] = character;
}
/*
 * Gets the position of a chosen label.
 * @param label The label to get the position of.
 * @return Returns the label's program counter position if found,
 * otherwise it will return -1.
 */
unsigned int getLabelPosition(char label[MAX_CMD_LEN])
{
	// Create the for loop i variable.
	int i = 0;
	/*
	 * Create a comparer string array.  This is used because
	 * the label will not have quotes on either side, and must be appended
	 * as a result.
	 */ 
	char comparer[MAX_CMD_LEN];
	// Loop through all of the commands.
	for(i = 0;i < MAX_COMMANDS;i += 1)
	{
		// If the first arguments first character is a ':'
		// then it is a label.
		if(commands[i].args[0][0] == ':')
		{
			// Set all of the comparer variables values to '\0' (0).
			memset(comparer, '\0', MAX_CMD_LEN);
			// Add the first " to the comparer.
			strncat(comparer, "\"", 1);
			// Add the command to the comparer. (The label's name in this case)
			strncat(comparer, commands[i].cmd, strlen(commands[i].cmd));
			// Add the last " to the comparer.
			strncat(comparer, "\"", 1);
			// Check if the comparer and the label are equal.
			if(!strcmp(comparer, label))
			{
				// Return the index of the command.  This
				// is techincally a program counter value.
				return i;
			}
		}
	}
	// Return -1 if no label was found.
	return -1;
}

/*
 * Runs a command through the interpreter.
 * @param showDebug Set to true to print debug info with the command.
 * @param cmd The command to run.
 * @param args The arguments to use with the command.
 * @return Returns 0 upon success, 1 if the program should end, and
 * 0 if there was an error.
 */
int runCommand(bool showDebug, char cmd[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN])
{
	// Check if argument 1 has a register and argument 2 has a memory location.
	if((args[0][0] == 'r') && (args[1][0] == 'm'))
	{
		// If so, set the register and value.
		int r1 = args[0][1] - '0';
		int m1 = args[1][1] - '0';
		// Check if the command is the load command.
		if(!strcmp(cmd, "load"))
		{
			// Then call the add command with those values.
			load(r1, m1);
		}
	}
	// Check if argument 1 has a memory location and argument 2 has a register.
	else if((args[0][0] == 'm') && ((args[1][0] == 'r') || (args[1][0] == 'a')))
	{
		// If so, set the register and value.
		int m1 = args[0][1] - '0';
		int r1 = args[1][1] - '0';
		if(args[1][1] == 'c')
		{
			r1 = -1;
		}
		// Check if the command is the add command.
		if(!strcmp(cmd, "store"))
		{
			// Then call the add command with those values.
			store(m1, r1);
		}
	}
	// Check if argument 1 and argument 2 has a register.
	else if(((args[0][0] == 'r') || (args[0][0] == 'a')) && ((args[1][0] == 'r') || (args[1][0] == 'a')))
	{
		// If so, set the register and value.
		int r1 = args[0][1] - '0';
		int r2 = args[1][1] - '0';
		if(args[0][1] == 'c')
		{
			r1 = -1;
		}
		if(args[1][1] == 'c')
		{
			r2 = -1;
		}
		// Check if the command is the add command.
		if(!strcmp(cmd, "add"))
		{
			// Then call the add command with those values.
			add(r1, r2);
		}
		// Check if the command is the sub command.
		if(!strcmp(cmd, "sub"))
		{
			// Then call the sub command with those values.
			sub(r1, r2);
		}
		// Check if the command is the mul command.
		if(!strcmp(cmd, "mul"))
		{
			// Then call the mul command with those values.
			mul(r1, r2);
		}
		// Check if the command is the div command.
		if(!strcmp(cmd, "div"))
		{
			// Then call the div command with those values.
			divide(r1, r2);
		}
		// Check if the command is the mod command.
		if(!strcmp(cmd, "mod"))
		{
			// Then call the mod command with those values.
			mod(r1, r2);
		}
		// Check if the command is the mov command.
		if(!strcmp(cmd, "mov"))
		{
			// Then call the copy command with those values since mov and copy are the same here.
			copy(r1, r2);
		}
		// Check if the command is the copy command.
		if(!strcmp(cmd, "copy"))
		{
			// Then call the copy command with those values.
			copy(r1, r2);
		}
		// Check if the command is the cmp command.
		if(!strcmp(cmd, "cmp"))
		{
			// Then call the cmp command with those values.
			cmp(r1, r2);
		}
	}
	// Check if argument 1 has the register and argument 2 isn't empty.
	else if(args[0][0] == 'r' && args[1][0] != '\0')
	{
		// If so, set the register and value.
		int r1 = args[0][1] - '0';
		// Check if the command is the set command.
		if(!strcmp(cmd, "set"))
		{
			int value = 0;
			// Check if there is a number sign as
			// the first character in argument 1.
			if(args[1][0] == '#')
			{
				// If so, use sscanf with # first.
				sscanf(args[1], "#%d", &value);
			}
			else
			{
				// Otherwise, use sscanf without # first.
				sscanf(args[1], "%d", &value);
			}
			// Then call the set command with those values.
			set(r1, value);
		}
		// Check if the command is the mov command.
		if(!strcmp(cmd, "mov"))
		{
			int value = 0;
			// Check if there is a number sign as
			// the first character in argument 1.
			if(args[1][0] == '#')
			{
				// If so, use sscanf with # first.
				sscanf(args[1], "#%d", &value);
			}
			else
			{
				// Otherwise, use sscanf without # first.
				sscanf(args[1], "%d", &value);
			}
			// Then call the set command with those values since mov and set are the same here.
			set(r1, value);
		}
		// Check if the command is the set command.
		if(!strcmp(cmd, "jeq"))
		{
			// Get the position of the label to jump to.
			unsigned int pos = getLabelPosition(args[1]);
			if(pos < 0)
			{
				return -1;
			}
			// Then call the jmp command with those values.
			jeq(r1, pos);
		}
		// Check if the command is the set command.
		if(!strcmp(cmd, "jne"))
		{
			// Get the position of the label to jump to.
			unsigned int pos = getLabelPosition(args[1]);
			if(pos < 0)
			{
				return -1;
			}
			// Then call the jmp command with those values.
			jne(r1, pos);
		}
	}
	// Check if at least argument 1 has a register.
	else if(((args[0][0] == 'r') || (args[0][0] == 'a')) && ((strstr(cmd, "print")) || (!strcmp(cmd, "getc"))))
	{
		// If so, only set the register.
		int r1 = args[0][1] - '0';
		if(args[0][1] == 'c')
		{
			r1 = -1;
		}
		// Check if the command is the print command.
		if(!strcmp(cmd, "print"))
		{
			// Then call the print command with those values.
			print(r1);
		}
		// Check if the command is the printc command.
		if(!strcmp(cmd, "printc"))
		{
			// Then call the printc command with those values.
			printc(r1);
		}
		// Check if the command is the print command.
		if(!strcmp(cmd, "getc"))
		{
			// Then call the print command with those values.
			getcFunc(r1);
		}
	}
	// Check if the first arguement at least has text.
	else if(args[0][0] != '\0')
	{
		// Check if the command is the jmp command.
		if(!strcmp(cmd, "jmp"))
		{
			// Get the position of the label to jump to.
			unsigned int pos = getLabelPosition(args[0]);
			if(pos < 0)
			{
				return -1;
			}
			// Then call the jmp command with those values.
			jmp(pos);
		}
	}
	else
	{
		// Check if the command is the jmp command.
		if(!strcmp(cmd, "halt"))
		{
			return 1;
		}
		if(showDebug)
		{
			iprintf("Error! Command %s with first argument %s is invalid!\n", cmd, args[0]);
		}
		// If none of the above possiblities are true, then an invalid command was given.
		return -1;
	}

	// Increment the program counter.
	pc += 1;
	// Return 0 since it was successful and there was no jumping.
	return 0;
}



