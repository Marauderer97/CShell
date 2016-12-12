#include <sys/wait.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int bgflag;
int TBS=64;
int BS=1024;
char shell_delimiter[20]=" \n\t\r\a";
char semic_delimiter[10]=";";
char pipe_delimiter[10]="|";
char homed[100];
char outfile[100];char infile[100];char appfile[100];
int inflag;int outflag;int appflag;int dflag;
char * s_read()
{
	int bs=BS;
	int p=0;
	char * buffer = malloc(sizeof(char)*bs);
	if(!buffer)
	{
		fprintf(stderr, "memory alloc error\n");
		exit(EXIT_FAILURE);
	}
	int c;
	while(1)
	{
		c = getchar();
		//              printf("got char\n");
		if(c== EOF || c =='\n')
		{
			if(c == EOF)
				exit(1);
			buffer[p]='\0';
			return buffer;
		}
		else
			buffer[p]=c;
		p++;
	}
	if(p>= bs)
	{
		bs+=BS;
		buffer = realloc(buffer, bs);
		{
			fprintf(stderr, "memory alloc error\n");
			exit(EXIT_FAILURE);
		}
	}
}
char ** s_tokenize(char * line)
{
	int bs=TBS;
	int p=0;
	char **tokens = malloc(bs * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, shell_delimiter);
	while (token != NULL) {
		if(strcmp(token,"&")==0)
			bgflag=1;
		else if (strcmp(token,">")==0)
		{
			outflag=1;
			token = strtok(NULL, shell_delimiter);
			if (token==NULL)
			{
				outflag=0;
				fprintf(stderr, "Wrong usage of >\n");
				exit(EXIT_FAILURE);
			}
			else 
			{	strcpy(outfile,token);
			}
		}
		else if (strcmp(token, "<")==0)
			{
				inflag=1;
				token = strtok(NULL, shell_delimiter);
				if (token==NULL)
			{
				inflag=0;
				fprintf(stderr, "Wrong usage of <\n");
				exit(EXIT_FAILURE);
			}
			else 
			{	strcpy(infile,token);
			}
		}

		else if (strcmp(token,">>")==0)
			{
				appflag=1;
				token = strtok(NULL, shell_delimiter);
							if (token==NULL)
			{
				appflag=0;
				fprintf(stderr, "Wrong usage of >>\n");
				exit(EXIT_FAILURE);
			}
			else 
			{	strcpy(appfile,token);

			}
		}
		else
		{
			tokens[p] = token;
			p++;
		}

		if (p>bs) {
			bs+=TBS;
			tokens = realloc(tokens, bs * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, shell_delimiter);
	}
	tokens[p] = NULL;
	return tokens;
}


char ** s_tokenize2(char * line)
{
	int bs=TBS;
	int p=0;
	char **tokens = malloc(bs * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, semic_delimiter);
	while (token != NULL) {
		tokens[p] = token;
		p++;

		if (p>bs) {
			bs+=TBS;
			tokens = realloc(tokens, bs * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, semic_delimiter);
	}
	tokens[p] = NULL;
	return tokens;
}
char ** s_ptokenize(char * line)
{
	int bs=TBS;
	int p=0;
	char **tokens = malloc(bs * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "Allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, pipe_delimiter);
	while (token != NULL) {
		tokens[p] = token;
		p++;

		if (p>bs) {
			bs+=TBS;
			tokens = realloc(tokens, bs * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "Allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, pipe_delimiter);
	}
	tokens[p] = NULL;
	return tokens;
}
