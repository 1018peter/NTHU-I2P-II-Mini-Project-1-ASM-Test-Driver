#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma warning (disable : 4996)


typedef enum { UNDEFINED, REGISTER, MEMORY, CONSTANT } argType;

int main(int argc, char** argv) {
	if (argc > 5) {
		printf("Too many arguments!\n");
		return 0;
	}
	char instruction[100];
	char argument[100];
	int memory[64];
	int mem_init[64] = { 0 };
	mem_init[0] = 1;
	mem_init[1] = 1;
	mem_init[2] = 1;
	if (argc >= 2) memory[0] = atoi(argv[1]);
	else memory[0] = 57;
	if (argc >= 3) memory[1] = atoi(argv[2]);
	else memory[1] = 23;
	if (argc >= 4) memory[2] = atoi(argv[3]);
	else memory[2] = 101;

	int reg[8];
	int reg_init[8] = { 0 };
	int clock_cycle = 0;




	int output_mode = 0;
	if (argc == 5) {
		output_mode = atoi(argv[4]);
	}

	printf("Processing instructions...\n");
	while (fscanf(stdin, "%s", instruction) != EOF) {

		char cursor = getchar();
		argType target_type = UNDEFINED;
		int target_id = 0;


		while (cursor == ' ') {
			cursor = getchar();
		}
		if (cursor == 'r') {
			target_type = REGISTER;
			fscanf(stdin, "%d", &target_id);
		}
		else if (cursor == '[') {
			target_type = MEMORY;
			fscanf(stdin, "%d", &target_id);
			if (target_id % 4 != 0) {
				printf("ERROR: Memory access misaligned.\n");
				break;
			}
			if (getchar() != ']') {
				printf("ERROR: Invalid syntax.\n");
				break;
			}
			cursor = getchar();
		}
		else if (cursor == '1' && strcmp(instruction, "EXIT") == 0) {
			printf("Exiting process with status 1.\n");
			clock_cycle += 20;
			break;
		}
		else if (cursor == '0' && strcmp(instruction, "EXIT") == 0) {
			printf("Exiting process with status 0.\n");
			clock_cycle += 20;
			break;
		}
		else {
			printf("ERROR: Invalid argument.\n");
			break;
		}

		cursor = getchar();

		argType source_type = UNDEFINED;
		int source_id = 0;
		while (cursor == ' ') {
			cursor = getchar();
		}
		if (cursor == 'r') {
			source_type = REGISTER;
			fscanf(stdin, "%d", &source_id);
		}
		else if (cursor == '[') {
			source_type = MEMORY;
			fscanf(stdin, "%d", &source_id);
			if (source_id % 4 != 0) {
				printf("ERROR: Memory access misaligned.\n");
				break;
			}
			if (getchar() != ']') {
				printf("ERROR: Invalid syntax.\n");
				break;
			}
			cursor = getchar();
		}
		else if (isdigit(cursor)) {
			source_type = CONSTANT;
			char constant_string[100];
			int strcursor = 0;
			while (isdigit(cursor)) {
				constant_string[strcursor++] = cursor;
				cursor = getchar();
			}
			constant_string[strcursor] = '\0';
			source_id = atoi(constant_string);


		}
		else {
			printf("ERROR: Unrecognized argument.\n");
			break;
		}

		if (strcmp(instruction, "MOV") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				reg[target_id] = reg[source_id];
				reg_init[target_id] = 1;
				clock_cycle += 10;

			}
			else if (target_type == REGISTER && source_type == MEMORY) {
				if (mem_init[source_id / 4] == 0) {
					printf("ERROR: Trying to read from uninitialized memory %d\n", source_id / 4);
				}
				reg[target_id] = memory[source_id / 4];
				reg_init[target_id] = 1;
				clock_cycle += 200;
			}
			else if (target_type == MEMORY && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				memory[target_id / 4] = reg[source_id];
				mem_init[target_id / 4] = 1;
				clock_cycle += 200;
			}
			else if (target_type == REGISTER && source_type == CONSTANT) {
				reg[target_id] = source_id;
				reg_init[target_id] = 1;
				clock_cycle += 10;

			}
			else {
				printf("ERROR: Invalid MOV operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "ADD") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				reg[target_id] += reg[source_id];
				clock_cycle += 10;
			}
			else {
				printf("ERROR: Invalid ADD operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "SUB") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				reg[target_id] -= reg[source_id];
				clock_cycle += 10;
			}
			else {
				printf("ERROR: Invalid SUB operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "MUL") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				reg[target_id] *= reg[source_id];
				clock_cycle += 30;
			}
			else {
				printf("ERROR: Invalid MUL operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "DIV") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				if (reg[source_id] == 0) {
					printf("ERROR: Division by zero.\n");
					break;
				}
				reg[target_id] /= reg[source_id];
				clock_cycle += 50;
			}
			else {
				printf("ERROR: Invalid ADD operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "AND") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				reg[target_id] &= reg[source_id];
				clock_cycle += 10;
			}
			else {
				printf("ERROR: Invalid AND operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "XOR") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				reg[target_id] ^= reg[source_id];
				clock_cycle += 10;
			}
			else {
				printf("ERROR: Invalid XOR operation.\n");
				break;
			}
		}
		else if (strcmp(instruction, "OR") == 0) {
			if (target_type == REGISTER && source_type == REGISTER) {
				if (reg_init[source_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", source_id);
					break;
				}
				if (reg_init[target_id] == 0) {
					printf("ERROR: Trying to read from uninitialized register %d\n", target_id);
					break;
				}
				reg[target_id] |= reg[source_id];
				clock_cycle += 10;
			}
			else {
				printf("ERROR: Invalid OR operation.\n");
				break;
			}
		}
		else {
			printf("ERROR: Unrecognized instruction.\n");
			break;
		}
	}
	printf("\n= Results =\n");

	if (output_mode == 0) {
		if (reg_init[0]) printf("r0 = %d\n", reg[0]);
		else printf("r0 is uninitialized.\n");
		if (reg_init[1]) printf("r1 = %d\n", reg[1]);
		else printf("r1 is uninitialized.\n");
		if (reg_init[2]) printf("r2 = %d\n", reg[2]);
		else printf("r2 is uninitialized.\n");
	}
	else {
		printf("== Register Info ==\n");
		for (int i = 0;i < 8;i++) {
			if (reg_init[i])printf("r%d = %d\n", i, reg[i]);
			else printf("r%d is uninitialized.\n", i);
		}
		printf("\n");
		printf("== Memory Info ==\n");
		for (int i = 0;i < 64;i++) {
			if (mem_init[i]) printf("[%d] = %d\n", i * 4, memory[i]);
			else printf("[%d] is uninitialized.\n", i * 4);
		}
		printf("\n");
	}

	printf("Total Clock Cycles: %d\n", clock_cycle);

	return 0;
}