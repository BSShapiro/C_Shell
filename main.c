#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct Command{
	char *name;
	int code;
};

struct Command dict[] = {
	{"error", -1},
	{"exit", 0},
	{"ls", 2}
};

int parser(char* cmd){
	char command[255];

	sscanf(cmd, "%s", command);
	
	for (unsigned long i = 0; i < sizeof(dict)/sizeof(dict[0]); i++){
		if(strcmp(dict[i].name, command) == 0){
			return dict[i].code;
		}
	}
	return -1;
}

int main(){
	char input[1024];
	int opcode = 1;
	while (opcode != 0){
		printf("shapsh>> ");
			
		fgets(input, sizeof(input), stdin);

		input[strcspn(input, "\r\n")] = '\0';

		for(unsigned long i = 0; i < strlen(input); i++){
			input[i] = tolower((unsigned char)input[i]);
		}

		if (input[0] != '\0'){
			opcode = parser(input);
				
		}

		if (opcode == -1){
			printf(ANSI_COLOR_RED "An error has occured. Type help for more info." ANSI_COLOR_RESET);
		}
	}
	return 0;
}
