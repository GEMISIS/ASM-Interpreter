#ifndef _INTERPRETER_DEBUGPRINTER_H_
#define _INTERPRETER_DEBUGPRINTER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include <stdio.h>

// Include functions for the interpreter.
#include "interpreter_cmdReader.h"

// The border to display on the top screen.
extern const char topScreenBorderText[];

// Create a top screen screen console for text.
extern PrintConsole topScreenBorder, topScreenPC, topScreenMemory, topScreenRegisters;

/*
 * Initializes the system for the debug printer.
 */
void initDebugPrinter();
/*
 * Prints the border on the top screen of the Nintendo DS.
 */
void printTopScreenBorder();
/*
 * Prints the memory, registers, and program counter
 * for the interpreter.
 */
void printInterpreterSystemInfo();

#ifdef __cplusplus
}
#endif

#endif


