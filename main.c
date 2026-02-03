#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h> 
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char cwd[1024];

//command struct
struct Command{
	char *name;
	int code;
};

//array of stucts to store commands and their opcodes
struct Command dict[] = {
	{"error", -1},
	{"exit", 0},
	{"ls", 2},
	{"pwd", 3},
	{"clear", 4}
};

// The "Comparison" rule for alphabetical order
int compare_names(const void *a, const void *b) {
    // We have to cast the void pointers back to char** // and then dereference them to get the actual strings.
    const char *name1 = *(const char **)a;
    const char *name2 = *(const char **)b;
    return strcmp(name1, name2);
}


//function to handle ls
int handle_ls(char* remaining){
	//ls with sub commands
	if (remaining != NULL){

	//this is for ls
	} else if(remaining == NULL){
			//directory array
			char **dir_arr;

			//directory stream pointer
			DIR *dir_ptr;

			//directory entry struct
			struct dirent *entry;

			//max array size
			int current_max = 25;

			//allocate memory for the dynamic array
			dir_arr = malloc(current_max * sizeof(char *));

			//if malloc fails return
			if(dir_arr == NULL){
				perror(ANSI_COLOR_RED "A call to malloc has failed" ANSI_COLOR_RESET);
				return -1;
				
				//else continue
			} else{
				//open the current directory
					dir_ptr = opendir(cwd);
					
					if(dir_ptr == NULL){
						perror(ANSI_COLOR_RED "The directory could not be opened." ANSI_COLOR_RESET);
					}
					int i = 0;
					while((entry = readdir(dir_ptr)) != NULL){
							
						if(i == current_max){
							current_max += 25;
							char **temp = realloc(dir_arr, current_max * sizeof(char *));

							if(temp == NULL){
								perror(ANSI_COLOR_RED "A call to malloc has failed" ANSI_COLOR_RESET);
								return -1;
							} else{
								dir_arr = temp;
							}
						}

						dir_arr[i] = strdup(entry->d_name);

						i++;
					}
					//sort dir_arr
					qsort(dir_arr, i, sizeof(char *), compare_names);

					//print elements of array and free
					for(int j = 0; j < i; j++){
					// i is your counter from the loop (total number of files found)
						if(dir_arr[j] != NULL){
							printf("%s\n", dir_arr[j]);	
						}
						free(dir_arr[j]);
					}
					closedir(dir_ptr);
					free(dir_arr);
			}
	}
	return 0;
}

//print working directory
void handle_pwd(){
	printf("%s", cwd);
}

//function to parse the command from user input
int parser(char* cmd){
	//store the command locally
	char command[255];

	//opcode variable
	int opcode;

	//get the base command
	sscanf(cmd, "%s", command);
	
	//get the rest of the command
	char *remaining_cmd = strchr(cmd, ' ');

	//grab the rest of the command using some pointer arithmetic
	if(remaining_cmd != NULL){
		remaining_cmd++;
	}
	
	//loop through the array of structs to find the command	
	for (unsigned long i = 0; i < sizeof(dict)/sizeof(dict[0]); i++){
		if(strcmp(dict[i].name, command) == 0){
			opcode =  dict[i].code;
		}
	}

	//if opcode is less than 1 (exit or error) then return
	if(opcode < 1){
		return opcode;
		
		//if opcode is anything else, send it to its appropriate function for handling
	} else if(opcode == 2) {
		handle_ls(remaining_cmd);
	} else if(opcode == 3){
		handle_pwd();
	}
	return 1;
}


//main function
int main(){
	//get home environment variable
	char *home = getenv("HOME");

	//bounds checking the home variable
	 if(strlen(home) < 1000){
	 	strncat(cwd, home, strlen(home));
	 } else{
	 	printf(ANSI_COLOR_RED "A fatal error has occured. The specified home path is too long. Exiting..." ANSI_COLOR_RESET);
	 	return -1;	
	 }
	
	
	//variable to store user input
	char input[1024];

	//default opcode is 1
	int opcode = 1;

	//loop until exit code is passed
	while (opcode != 0){
	
		//show the user that the shell is active
		printf("\nshapsh@%s>> ", cwd);

		//use fgets to get user input from standard input the safe way
		fgets(input, sizeof(input), stdin);

		//remove the newline character from the buffer and replace with a null terminator
		input[strcspn(input, "\r\n")] = '\0';

		//loop to convert user input to lowercase
		for(unsigned long i = 0; i < strlen(input); i++){
			input[i] = tolower((unsigned char)input[i]);
		}

		//if user input isn't blank, pass user input to the input function
		if (input[0] != '\0'){
			opcode = parser(input);
				
		}

		//show error for unknown command
		if (opcode == -1){
			printf(ANSI_COLOR_RED "An error has occured. Type help for more info.\n" ANSI_COLOR_RESET);
		}
	}
	return 0;
}
