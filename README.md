# NTHU-I2P-II-Mini-Project-1-ASM-Test-Driver
A test driver that lets people count clock cycles and debug more easily. Written in C.

The program emulates the CPU given in the project announcement, with 8 available registers (r0 ~ r7) and 256 bytes of available memory space.

Given a series of instructions in Assembly, terminated with an EXIT instruction, the program will output the results of the operations.

The program can take in a total of four arguments when executed in command line, specified as follows:
./ASMTestDriver [x] [y] [z] [output_mode]
x, y, z: Corresponds to the initial value in the memory spaces [0], [4], [8]. By default, it is set to three numbers: 57, 23, 101.
output_mode: 0 by default. If 0, the output will be in a concise form, consisting only of info regarding the registers r0, r1, r2, and the total number of clock cycles. If 1, the output will be in a detailed form, listing info about all registers and all memory spaces.

The program will terminate early if it encounters errors such as accessing illegal registers and memory spaces or dividing by zero, or if it encounters an instruction it cannot recognize.

The instructions supported are MOV, ADD, SUB, MUL, DIV, AND, XOR, OR, and EXIT.
