#include "interpreter_cmdReader.h"

// An enumorator for what states 
// the interpreter can be in.
enum interpreterState_t
{
	// Reading the command.
	CMD = 0,
	// Reading the 1st argument.
	ARG1 = 1,
	// Reading the 2nd argument.
	ARG2 = 2,
	// Reading the 3rd argument.
	ARG3 = 3,
	// Validating and Storing the command.
	VANDS = -1,
	// Reading a comment.
	COMMENT = -2
};

/*
 * Adds the command and its arguments to the command at the program counter.
 * @param cmd The command to add to the list.
 * @param args The commands arguments to be passed.
 */
void addCommandToList(char cmd[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN])
{
	// For loop variable.
	int i;
	// Copy the command itself to the command.
	strncpy(commands[pc].cmd, cmd, MAX_CMD_LEN);
	// Loop through all of the arguments...
	for(i = 0;i < MAX_ARGS;i += 1)
	{
		// and add them to the command.
		strncpy(commands[pc].args[i], args[i], MAX_ARG_LEN);
	}
}

/*
 * Validates and stores a command and returns whether it was successful.
 * @param cmd The command to store.
 * @param args The commands arguments to store.
 * @return Returns 0 if successful, -1 otherwise.
 */
int ValidateAndStoreCommand(char cmd[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN])
{
	// Check if argument 1 has a register and argument 2 has a memory location.
	if((args[0][0] == 'r') && (args[1][0] == 'm'))
	{
		// Check if the command is the load command.
		if(!strcmp(cmd, "load"))
		{
			// Add the command to the list of commands.
			addCommandToList(cmd, args);
		}
	}
	// Check if argument 1 has a memory location and argument 2 has a register.
	else if((args[0][0] == 'm') && ((args[1][0] == 'r') || (args[1][0] == 'a')))
	{
		// Check if the command is the load command.
		if(!strcmp(cmd, "store"))
		{
			// Add the command to the list of commands.
			addCommandToList(cmd, args);
		}
	}
	// Check if argument 1 and argument 2 has a register.
	else if(((args[0][0] == 'r') || (args[0][0] == 'a')) && ((args[1][0] == 'r') || (args[1][0] == 'a')))
	{
		// Check if the command is a valid command.
		if(!strcmp(cmd, "add") || !strcmp(cmd, "sub") || !strcmp(cmd, "mul")
			|| !strcmp(cmd, "div") || !strcmp(cmd, "mod") || !strcmp(cmd, "copy")
			|| !strcmp(cmd, "cmp") || !strcmp(cmd, "mov"))
		{
			// Add the command to the list of commands.
			addCommandToList(cmd, args);
		}
	}
	// Check if argument 1 has the register and argument 2 isn't empty.
	else if(((args[0][0] == 'r') || (args[0][0] == 'a')) && args[1][0] != 0)
	{
		// Check if the command is valid command.
		if(!strcmp(cmd, "set") || !strcmp(cmd, "mov") || !strcmp(cmd, "jeq") || !strcmp(cmd, "jne"))
		{
			// Add the command to the list of commands.
			addCommandToList(cmd, args);
		}
	}
	// Check if argument 1 has the colon character, which means it is a label.
	else if(args[0][0] == ':')
	{
		args[1][0] = pc;
		// Add the command to the list of commands.
		addCommandToList(cmd, args);
	}
	// Check if the first arguement at least has text.
	else if(args[0][0] != '\0')
	{
		// Check if the command is the jmp command.
		if(!strcmp(cmd, "jmp") || !strcmp(cmd, "print") || !strcmp(cmd, "printc") || !strcmp(cmd, "getc"))
		{
			// Add the command to the list of commands.
			addCommandToList(cmd, args);
		}
	}
	else if(!strcmp(cmd, "halt"))
	{
		// Add the command to the list of commands.
		addCommandToList(cmd, args);
	}
	else
	{
		// If none of the above possiblities are true, then an invalid command was given.
		return -1;
	}

	// Increment the program counter.
	pc += 1;
	// Return 0 since it was successful and there was no jumping.
	return 0;
}

/*
 * Loads a program into the commands array.
 * @param showDebug Whether to print debug info to the screen
 * when loading commands.
 * @param program The commands arguments to store.
 * @return Returns 0 if successful, -1 otherwise.
 */
int loadProgram(bool showDebug, char* program)
{
	// The position of the program that is currently being read.
	int position = 0;
	// The current command and its arguments.
	char cmd[MAX_CMD_LEN], args[MAX_ARGS][MAX_ARG_LEN];
	// The currently read character.
	char character = ' ';
	// The current state of the interpreter.
	enum interpreterState_t state = CMD;

	// The for loop variable for looping through arguments.
	int i = 0, j = 0, k = 0;

	pc = 0;
	for(i = 0;i < MAX_COMMANDS;i += 1)
	{
		for(j = 0;j < MAX_CMD_LEN;j += 1)
		{
			commands[i].cmd[j] = 0;
		}
		for(j = 0;j < MAX_ARGS;j += 1)
		{
			for(k = 0;k < MAX_ARG_LEN;k += 1)
			{
				commands[i].args[j][k] = 0;
			}
		}
	}

	// Set the command to be empty.
	memset(cmd, '\0', MAX_CMD_LEN);

	// Set each argument to be empty.
	for(j = 0;j < MAX_ARGS;j += 1)
	{
		memset(args[j], '\0', MAX_ARG_LEN);
	}

	// Reset the AC register.
	reg_ac = 0;

	// Resest the normal registers.
	for(j = 0;j < MAX_REG;j += 1)
	{
		registers[j] = 0;
	}
	// Reset the memory locations.
	for(j = 0;j < MAX_MEM;j += 1)
	{
		memory[j] = 0;
	}

	/*
	 * So long as there is a character to read and the state is not
	 * validating and storing, going through the program.
	 */
	while(character != 0 || state == VANDS)
	{
		// Check that the state is not validate and store.
		if(state != VANDS)
		{
			// If so, read the next character.
			character = program[0];
			// Then move to the next character.
			program += 1;
		}
		
		// Switch through the possible states.
		switch(state)
		{
		case CMD:
			// If we are reading a command, switch through the character.
			switch(character)
			{
			case '/':
				// For a /, we are reading a comment, and thus
				// set the position to 0 and set the state to
				// comment.
				position = 0;
				state = COMMENT;
				break;
			case ' ':
			case '\t':
				// For any kind of whitespace, we check
				// if the position is ont 0.
				if(position != 0)
				{
					// If it is, then we set the position to
					// 0, and the state to reading arguments.
					position = 0;
					state = ARG1;
				}
				break;
			case '\0':
			case '\n':
			case '\r':
				// For any kind of ending character, we check
				// if the position is ont 0.
				if(position != 0)
				{
					// If it is, we reset the position, and set the
					// state to validating and storing.
					position = 0;
					state = VANDS;
				}
				break;
			case ':':
				// If it is a colon, then the command is a lable.
				// We reset the position.
				position = 0;
				// Set the first argument first character to a colon.
				args[0][0] = ':';
				// Set the first arguments last character to \0 (this makes
				// it a C string).
				args[0][1] = '\0';

				// Then set the other arugments to be empty strings.
				for(j = 1;j < MAX_ARGS;j += 1)
				{
					memset(args[j], '\0', MAX_ARG_LEN);
				}

				// Finally set the state to validating and storing.
				state = VANDS;
				break;
			default:
				// Any other character is set to be stored in the command.
				cmd[position] = character;
				// Then the command position is updated.
				position += 1;
				// The the last character is set to be \0, making it a C string.
				cmd[position] = '\0';
				break;
			}
			break;
		default:
			// If the state is reading an argument, switch the character.
			switch(character)
			{
			case ' ':
			case '\t':
				// Whitespace is ignored.
				break;
			case ',':
				// , signify a new command, so the position is reset
				// and the state is incremented by 1.
				position = 0;
				state += 1;
				break;
			case '\0':
			case '\n':
			case '\r':
				// Any end of line characters resets the position
				// and sets the state to validating and storing.
				position = 0;
				state = VANDS;
				break;
			default:
				// Any other characters are stored in the
				// current argument.
				args[state - 1][position] = character;
				// Then the position is incremented by 1.
				position += 1;
				// The the last character is then set to be \0, making it a C string.
				args[state - 1][position] = '\0';
				break;
			}
			break;
		case VANDS:
			// If we are validating and store a command, then
			// we must call that method, and check that it is not less
			// than 0.
			if(ValidateAndStoreCommand(cmd, args) < 0)
			{
				// If it is, we print an error if debugging.
				if(showDebug)
				{
					iprintf("Error! Command %s at %d with first argument %s is invalid!\n", cmd, pc, args[0]);
				}
				// We also return -1 to say that there was an error.
				return -1;
			}

			// The command is set to an emtpy string.
			memset(cmd, '\0', MAX_CMD_LEN);
			// Each argument is set to an emtpy string.
			for(j = 0;j < MAX_ARGS;j += 1)
			{
				memset(args[j], '\0', MAX_ARG_LEN);
			}

			// The state is reset to the command reading state.
			state = CMD;
			break;
		case COMMENT:
			// If it is a comment, then we switch through the character.
			switch(character)
			{
			case '\0':
			case '\n':
			case '\r':
				// If the character is an end of line character, the state is
				// reset to the command reading state.
				state = CMD;
				break;
			}
			break;
		}
	}
	// Finally, 0 is returned upon success.
	return 0;
}

