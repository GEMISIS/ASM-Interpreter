// Assembly version
main:
	set r4,2
	set r5,30
	store m1,r4
	store m2,r5
pow:
	load r2,m1
	load r3,m2
	copy r2,r4
	copy r4,r5
	set r7,1
forJ:
	cmp r7,r3
	set r1,-1
	jne r1,"endForJ"
	set r6,1
forI:
	cmp r6,r2
	set r1,-1
	jne r1,"endForI"
	add r5,r4
	copy ac,r5
	set r1,1
	add r6,r1
	copy ac,r6
	jmp "forI"
endForI:
	copy r5,r4
	set r1,1
	add r7,r1
	copy ac,r7
	jmp "forJ"
endForJ:
	store m3,r5
	print r5
	set r1, 10
	printc r1
	halt

