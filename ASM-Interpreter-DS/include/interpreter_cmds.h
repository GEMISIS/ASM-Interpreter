#ifndef _INTERPRETER_CMDS_H_
#define _INTERPRETER_CMDS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include <stdio.h>

// The max amount of arguments that can be used in a command.
#define MAX_ARGS 3
// The max length that a command can be.
#define MAX_CMD_LEN 32
// The max length that an argument can be.
#define MAX_ARG_LEN 32

// The max amount of commands that can be called.
#define MAX_COMMANDS 512

// The max number of registers available.
#define MAX_REG 9
// The max number of memory locations available.
#define MAX_MEM 15

// The program counter.
extern unsigned int pc;
// The accumulator register.
extern s32 reg_ac;
// The array of registers.
extern s32 registers[MAX_REG];
// The array of memory locations.
extern s32 memory[MAX_MEM];

// A structure used to store commands to call.
typedef struct
{
	// The command that is to be executed.
	char cmd[MAX_CMD_LEN];
	// The arguments associated with this command.
	char args[MAX_ARGS][MAX_ARG_LEN];
}command_t;

// The list of commands to run.
extern command_t commands[MAX_COMMANDS];

/*
 * Add the values in registers r1 and r2 and store the value in the accumulator.
 * @param r1 The first register to add. (-1 to use the accumulator)
 * @param r2 The second register to add. (-1 to use the accumulator)
 */
void add(int r1, int r2);
/*
 * Subtract the values in registers r1 and r2 from each other and store the value in the accumulator.
 * @param r1 The register to subtract from. (-1 to use the accumulator)
 * @param r2 The register to subtract with. (-1 to use the accumulator)
 */
void sub(int r1, int r2);
/*
 * Multiply the values in registers r1 and r2 and store the value in the accumulator.
 * @param r1 The first register to multiply. (-1 to use the accumulator)
 * @param r2 The second register to multiply. (-1 to use the accumulator)
 */
void mul(int r1, int r2);
/*
 * Divide the values in registers r1 and r2 and store the value in the accumulator.
 * @param r1 The number to divide. (-1 to use the accumulator)
 * @param r2 The number to divide by. (-1 to use the accumulator)
 */
void divide(int r1, int r2);
/*
 * Perform the modulous function with the two registers and store the value 
 * @param r1 The number to use the modulous on. (-1 to use the accumulator)
 * @param r2 The modulous value to use. (-1 to use the accumulator)
 */
void mod(int r1, int r2);
/*
 * Store the value in the chosen register to a memory location.
 * @param m1 The memory location to store the value in.
 * @param r1 The register to get the value from. (-1 to use the accumulator)
 */
void store(int m1, int r1);
/*
 * Loads the value in a chosen memory location to a register.
 * @param r1 The register to store the value to. Note that you cannot overwrite
 * the accumulator, so -1 is not a valid value.
 * @param m1 The memory location to get the value from.
 */
void load(int r1, int m1);
/*
 * Copies the value in the first register to the second register.
 * @param r1 The register to load the value from. (-1 to use the accumulator)
 * @param r2 The register to store the value to. Note that you cannot overwrite
 * the accumulator, so -1 is not a valid value.
 */
void copy(int r1, int r2);
/*
 * Jumps to a specific position in the program. (IE: Sets the program counter)
 * @param label The position to jump to.
 */
void jmp(unsigned int label);
/*
 * Sets a register to a specific value.
 * @param r1 The register to set.
 * @param value The value to set the register to.
 */
void set(int r1, s32 value);
/*
 * Jumps if the register does not have the same value as the accumulator.
 * @param r1 The register to check the value of.
 * @param label The label to jump to.
 */
void jne(int r1, unsigned int label);
/*
 * Jumps if the register has the same value as the accumulator.
 * @param r1 The register to check the value of.
 * @param label The label to jump to.
 */
void jeq(int r1, unsigned int label);
/*
 * Compares two registers and stores the comparison value in the accumulator.
 * -1 if the first register is less than the second, 1 if the first register
 * is greater than the second, and 0 if they are equal.
 * @param r1 The first register to check the value of.
 * @param r2 The second register to check the value of.
 */
void cmp(int r1, int r2);
/*
 * Prints the value of the register as an integer.
 * @param r1 The register to print the value of. (-1 to use the accumulator)
 */
void print(int r1);
/*
 * Prints the value of the register as an character.
 * @param r1 The register to print the value of. (-1 to use the accumulator)
 */
void printc(int r1);
/*
 * Gets a character from the keyboard.  On the Nintendo DS, this is done
 * via a keyboard that appears on the bottom screen.
 * @param r1 The register to store the value in. (-1 to use the accumulator)
 */
void getcFunc(int r1);
/*
 * Gets the position of a chosen label.
 * @param label The label to get the position of.
 * @return Returns the label's program counter position if found,
 * otherwise it will return -1.
 */
unsigned int getLabelPosition(char label[MAX_CMD_LEN]);
/*
 * Runs a command through the interpreter.
 * @param showDebug Set to true to print debug info with the command.
 * @param cmd The command to run.
 * @param args The arguments to use with the command.
 * @return Returns 0 upon success, 1 if the program should end, and
 * 0 if there was an error.
 */
int runCommand(bool showDebug, char cmd[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN]);

#ifdef __cplusplus
}
#endif

#endif
