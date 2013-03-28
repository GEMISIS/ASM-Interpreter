// Main entrance to this program.
main:
	// Set the default input character to -1.
	set r1, -1

	// Set the line count to 0.
	set r2, 0
	// Set the word count to 0.
	set r3, 0
	// Set the character count to 0.
	set r4, 0

	// Set the state to NOTIN_WORD (0).
	set r5, 0

	// Set the temp register to 0.
	set r6, 0

	// Set the 0 valued register to 0.
	set r7, 0
	// Set the 1 valued register to 1.
	set r8, 1

// The beginning of the main loop.
loopWords:
	// Get the character
	getc r1

// Check that the character is not -1.
loopCheck:
	// Add 1 (r8) to character (r1)
	add r1, r8
	// Check if ac is 0 (r7) and jump to print results if so
	jeq r7, "printResults"

// Increment the number of characters.
newCharacter:
	// Add 1 to the character count.
	add r4, r8
	// Copy the result to the register.
	copy ac, r4

// Check whether the character is a newline character.
compareLines:
	// Set r6 to '\n' (10)
	set r6, 10
	// Subtract r6 from r1
	sub r1, r6
	// If ac is 0 (r7) jump to increment lines
	jeq r7, "incrementLines"

// Check if the character is a separator character.
compareSeparators:
	// Set r6 to ' ' (32)
	set r6, 32
	// Subtract r6 from r1
	sub r1, r6
	// Jump if ac is 0 (r7) to set not in word
	jeq r7, "setNotInWord"

	// Set r6 to '\t' (9)
	set r6, 9
	// Subtract r6 from r1
	sub r1, r6
	// Jump if ac is 0 (r7) to set not in word
	jeq r7, "setNotInWord"

	// Set r6 to '\n' (10)
	set r6, 10
	// Subtract r6 from r1
	sub r1, r6
	// Jump if ac is 0 (r7) to set not in word
	jeq r7, "setNotInWord"

// Check if the state is NOTIN_WORD (0)
compareNotInWord:
	// Subtract 0 (r7) from the state (r5)
	sub r5, r7
	// If the state is NOTIN_WORD (0), jump to increment words.
	jeq r7, "incrementWords"
	// Go to main loop
	jmp "loopWords"

// Increment the number of words.
incrementWords:
	// Set the state to IN_WORD (1)
	copy r8, r5
	// Add 1 to word count (r3)
	add r3, r8
	// Copy the result to word count (r3)
	copy ac, r3
	// Go to main loop
	jmp "loopWords"

// Set the state to NOTIN_WORD (0)
setNotInWord:
	// Set the state (r5) to NOTIN_WORD (0)
	copy r7, r5
	// Go to main loop
	jmp "loopWords"

// Increment the number of lines.
incrementLines:
	// Add 1 to line count (r2)
	add r2, r8
	// Copy the result to line count (r2)
	copy ac, r2
	// Jump to compare separators
	jmp "compareSeparators"

// Print the results to the screen.
printResults:
	// Set the temp register (r6) to '\t' (9)
	set r6, 9
	// Print the number of lines.
	print r2
	// Print r6 as a character.
	printc r6
	// Print the number of words.
	print r3
	// Print r6 as a character.
	printc r6
	// Print the number of characters.
	print r4
	// Set the temp register (r6) to '\n' (10)
	set r6, 10
	// Print a newline.
	printc r6
	halt