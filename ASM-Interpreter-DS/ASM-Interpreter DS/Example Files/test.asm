main:
	set r1, 2
	set r2, 3

mainCode:
	add r1, r2

label1:
	set r1, 10

	printc r1
	print ac
	printc r1

	set r3, 11

	jeq r3, "divider"

adder:
	add ac, r2
	jmp "label1"

divider:
	div ac, r3
	set r3, 1
	jeq r3, "end"

end:
	printc r1
	print ac
	printc r1
	printc r1
	halt