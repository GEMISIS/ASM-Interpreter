#ifndef _INTERPRETER_CMDREADER_H_
#define _INTERPRETER_CMDREADER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <nds.h>
#include <stdio.h>

#include "interpreter_cmds.h"

/*
 * Validates and stores a command and returns whether it was successful.
 * @param cmd The command to store.
 * @param args The commands arguments to store.
 * @return Returns 0 if successful, -1 otherwise.
 */
int ValidateAndStoreCommand(char cmd[MAX_CMD_LEN], char args[MAX_ARGS][MAX_ARG_LEN]);
/*
 * Loads a program into the commands array.
 * @param showDebug Whether to print debug info to the screen
 * when loading commands.
 * @param program The commands arguments to store.
 * @return Returns 0 if successful, -1 otherwise.
 */
int loadProgram(bool showDebug, char* program);

#ifdef __cplusplus
}
#endif

#endif
