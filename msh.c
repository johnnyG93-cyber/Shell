// The MIT License (MIT)
//
// Copyright (c) 2016 Trevor Bakker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
								// so we need to define what delimits our tokens.
								// In this case  white space
								// will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports four arguments

char history[15][MAX_COMMAND_SIZE] = {};
int history_idx = 0;
int max_val = 15;
int pids[15] = {};

int main(int argc, char* argv[])
{

	/*
		if( argc < 11 )
	{
	  printf("Error: You must provide 10 command line parameters for this program\n" );
	  printf("       You can run the program with: program_name [string] [number]\n");
	  exit(1);
	}
	*/



	// Allocate a string to hold the first parameter.  We could just use argv[1] as is, but
   // we will do it the longer way in case you need this functionality later.
   //char * string_param = (char*)malloc( strlen( argv[1] ) ); 

   // Copy the commandline string to our local variable
   //memcpy( string_param, argv[1], strlen( argv[1] ) );

   //this is where the above code ends for taking command line parameters
 /*
	//char   file_line[256]={};
   char   str[255]={};
   int    first_int;
   int    second_int;
   float  first_float;

   FILE * file;

   file = fopen( "sample.txt" , "r");

   if (file)
   {
	 // Use fscanf to parse each line of the file into variables.
	 while (fscanf(file, "%f %s %d %d", &first_float, &str[0], &first_int, &second_int )!=EOF)
	 {
	   // Print out what we've parsed
	   printf("Parsed: %f %s %d %d\n", first_float, str, first_int, second_int );
	 }

	 fclose(file);
   }
 */
 //above code is to read a file

	char* command_string = (char*)malloc(MAX_COMMAND_SIZE);

	while (1)
	{
		// Print out the msh prompt
		printf("msh> ");

		// Read the command from the commandline.  The
		// maximum command that will be read is MAX_COMMAND_SIZE
		// This while command will wait here until the user
		// inputs something since fgets returns NULL when there
		// is no input
		while (!fgets(command_string, MAX_COMMAND_SIZE, stdin));



		if (history_idx < max_val)
		{
			strcpy(history[history_idx], command_string);
		}
		else
		{
			for (int i = 0; i < 13; i++)
			{
				strcpy(history[i], history[i + 1]);
			}
			strcpy(history[14], command_string);
		}
		history_idx++;





		/* Parse input */
		char* token[MAX_NUM_ARGUMENTS];

		for (int i = 0; i < MAX_NUM_ARGUMENTS; i++)
		{
			token[i] = NULL;
		}


		int   token_count = 0;

		// Pointer to point to the token
		// parsed by strsep
		char* argument_ptr = NULL;

		char* working_string = strdup(command_string);

		// we are going to move the working_string pointer so
		// keep track of its original value so we can deallocate
		// the correct amount at the end
		char* head_ptr = working_string;

		// Tokenize the input strings with whitespace used as the delimiter
		while (((argument_ptr = strsep(&working_string, WHITESPACE)) != NULL) &&
			(token_count < MAX_NUM_ARGUMENTS))
		{
			token[token_count] = strndup(argument_ptr, MAX_COMMAND_SIZE);
			if (strlen(token[token_count]) == 0)
			{
				token[token_count] = NULL;
			}
			token_count++;
		}

		// Now print the tokenized input as a debug check
		// \TODO Remove this for loop and replace with your shell functionality

		//int token_index  = 0;
		int count = 0;
		//char exclamtion[100]={};

		if (token[0] == NULL)
		{
			continue;
		}




		//using strcmp to check for builtin commands
		if (strcmp(token[0], "quit") == 0)
		{
			exit(0);
		}

		if (strcmp(token[0], "exit") == 0)
		{
			exit(0);
		}
		else if (strcmp(token[0], "cd") == 0)
		{
			chdir(token[1]);
		}
		else if (strcmp(token[0], "history") == 0)
		{
			for (int i = 0; i < max_val; i++)
			{
				printf("[%d]  %s\n", count, history[i]);
				if (count < max_val)
				{
					count++;
				}
			}
		}
		else if (strcmp(token[0], "showpids") == 0)
		{
			for (int i = 0; i < max_val; i++)
			{
				printf("[%d]  %d\n", count, pids[i]);
				count++;
			}
		}
		else if (strcmp(token[0], "!") == 0)
		{
			printf("!");
		}
		else
		{

			pid_t pid1 = fork();
			if (pid1 == 0)
			{
				// Notice you can add as many NULLs on the end as you want



				int test = execvp(token[0], token);
				// if((execvp(token[0],token)!=-1))
				   //execvp(token[0],token);

				if ((test == -1))
				{
					printf("%s: Command not found.\n", token[0]);
					exit(0);
				}
				//}make test

			  /*
				for( token_index = 0; token_index < token_count-1; token_index ++ )
			  {
				printf(" %s",  token[token_index] );
			  }
			  printf(": Command not found.\n\n");
			  continue;
			  */



			}
			else
			{
				int status;
				wait(&status);



				if (history_idx < max_val)
				{
					pids[history_idx] = pid1;
				}
				else
				{
					pids[14] = pid1;
				}

			}

		}



		// Cleanup allocated memory
		for (int i = 0; i < MAX_NUM_ARGUMENTS; i++)
		{
			if (token[i] != NULL)
			{
				free(token[i]);
			}
		}

		free(head_ptr);

	}

	free(command_string);


	return 0;
	// e2520ca2-76f3-90d6-0242ac120003
}

