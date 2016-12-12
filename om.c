#include <sys/wait.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "headers.h"

typedef struct node{
	int proid;
	char cname[100];
	struct node * next;
}node;
node * head;
node * tail;

int inflag;int outflag;int appflag;int pipflag;
char outfile[100];char infile[100];char appfile[100];
char prompt[500]="<";
char prompt2[600];
char  hostname[100],username[100];
char currd[100];
char rs[100];
int bgflag=0;
char homed[100];

void strreplace(char * s)
{
	int i=0,j=0,flag=0;
	int k=0;
	int len=strlen(homed);
	if(strlen(s)>=len){
		for(j=0;j<len;j++)
			if(s[j]!= homed[j])
			{
				flag=1;
				break;
			}
		if(flag == 0)
		{
			rs[0]='/';
			rs[1]='~';
			k=2;
			i+=len;
		}
	}
	while(s[i]!='\0'){
		rs[k]=s[i];
		k++;
		i++;
	}
}

void shell_loop()
{
	int j=0;
	bgflag=0;inflag=0;outflag=0;
	char buff[10];
	char * input;
	char ** parts;
	char ** pparts;
	char ** args;
	int ws[50];
	int pipefd[100];
	int s,i,k,num,l;
	int status;
	while(1)
	{	  if (signal(SIGINT, SIGINT_handler) == SIG_ERR) {
          printf("SIGINT install error\n");
          exit(1);
     		}

		strcpy(prompt2, prompt);

		for(j=0;j<100;j++)
		{
			currd[j]='\0';
			rs[j]='\0';
		}
		getcwd(currd, sizeof(currd));
		strreplace(currd);
		strcat(prompt2, rs);
		strcat(prompt2, ">");
		printf("\n%s",prompt2);
		input = s_read();
		parts = s_tokenize2(input);//; tokenize
		i=0;
		while(parts[i]!=NULL){
			pparts = s_ptokenize(parts[i]);//| tokenize
			if(pparts[1]==NULL)// no | present
			{
				args =s_tokenize(parts[i]);	
					s=s_redirect(args);

				if(s<=0)
					return;
			}
			else
			{
				k=0;
				while(pparts[k+1]!=NULL)
					k++;
				num=k;

				    for(k = 0; k < num; k++){
        if(pipe(pipefd + k*2) < 0) {
            perror("couldn't pipe");
            exit(EXIT_FAILURE);
        }
    }
    k=0;
				while(pparts[k]!=NULL)
				{
				//	printf("%s\n",pparts[k]);
					args =s_tokenize(pparts[k]);
					//printf("%d %d %d %s %s\n",inflag, outflag, appflag, args[0], args[1] );	
					s=s_pexec(pparts,args,k,num,pipefd);
					inflag=outflag=appflag=0;
					//printf("%d %d %d %s %s\n",inflag, outflag, appflag, args[0], args[1] );
					if(s<=0)
						return;
					k++;
				}
		for(l=0;l<2*num;l++)
   			close(pipefd[l]);
   		for(l=0;l<(2*num)-1;l++)
   			wait(&status);

			} 
		i++;
		}
	}
	free(input);
	free(parts);
	free(pparts);
	free(args);
}

int main(int argc, char * argv)
{
	head=tail=NULL;
	getlogin_r(username, sizeof(username));
	gethostname(hostname,sizeof(hostname));
	strcat(prompt, username);
	strcat(prompt,"@");
	strcat(prompt,hostname);
	strcat(prompt,": ");
	getcwd(homed,sizeof(homed));
	//	printf("%s\n",prompt);

	shell_loop();

	return 0;

}
