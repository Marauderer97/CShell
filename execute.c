#include <sys/wait.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
typedef struct node{
	int proid;
	char** cargs;
	struct node * next;
}node;
node * head;
node * tail;
int bgflag;
char homed[100];
char outfile[100];char infile[100];char appfile[100];
int inflag;int outflag;int appflag;int pipflag;

void  SIGINT_handler(int sig)
{
     signal(sig, SIG_IGN);
     return;
}
int s_pother(char ** args)
{
	pipflag=0;
	execvp(args[0],args);
}

int s_other(char ** args)
{
	int pid,ppid,s,s2,i=0;
	//      while(args[i]!=NULL){
	pid=fork();

		if(pid==0)
		{
			if (execvp(args[0], args) == -1) {
				perror("could not execute command");
			}
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("error forking");
		} else {
	if(bgflag==0)
	{
			while(1) {
				waitpid(pid, &s, WUNTRACED);
				if (!(!WIFEXITED(s) && !WIFSIGNALED(s)))
					break;
					}
	}
	else if(bgflag==1)
	{
		bgflag=0;
		node* temp=(node *)malloc(sizeof(node));
		temp->proid=pid;
		temp->cargs=args;
		temp->next=NULL;
		if(head==NULL && tail == NULL)
			{
				head = temp;
				tail=temp;
				}
		else{
				tail->next=temp;
				tail=temp;
			}

		printf("The process %s with pid %d has started in the background\n",args[0],pid);
	}
}	
		while((ppid = waitpid(-1, &s, WNOHANG)) > 0) {
       // printf("PROCESS WITH PID-%d EXITED NORMALLY\n",ppid);
    	node* temp=head;
    	node* ptemp;
    		
    		if(temp!=NULL && temp->proid == ppid)
    		{
    				if(s==0)
    				printf("PROCESS %s WITH PID-%d EXITED NORMALLY\n",temp->cargs[0],ppid);
    				else 
    					printf("PROCESS %s WITH PID-%d WAS TERMINATED\n",temp->cargs[0],ppid);
  				head = temp->next;
    			free(temp);
    			if(head == NULL)
    				tail=NULL;
    		}
    		else
    		{
    			while(temp != NULL && temp->proid != ppid)
    			{
    				ptemp =temp;
    				temp=temp->next;
    			}
    			if (temp==NULL)
    			{
    				tail=ptemp;
    			}
    			else if(temp!=NULL)
    				ptemp->next=temp->next;
    			free(temp);
    	}
}

	return 1;
}
int s_help()
{
	fprintf(stdout,"This is the help page.\nBuiltin commands are help,exit,pwd,echo and cd.\nUse man pages for further help\n");
	return 1;
}

int s_exit()
{
	return 0;
}
int s_pwd()
{
	char currd[100];
	getcwd(currd, sizeof(currd));
	fprintf(stdout,"%s\n",currd);
	return 1;
}
int s_echo(char ** args)
{
	if(args[1]!=NULL)
		fprintf(stdout,"%s\n",args[1]);
	return 1;
}
int s_cd(char ** args)
{
	int r;
	char  u[100];
	getlogin_r(u,sizeof(u));
	//      printf("Entered s_cd\n");
	if(args[1]==NULL)
		perror("Enter cd <directory name>\n");
	else if(strcmp(args[1],"~")==0)
	{
		//		printf("%s\n",homed);
		r=chdir(homed);
		if (r!=0)
			perror("Could not enter directory\n");
	}

	else{
		//		printf("%s\n",args[1]);
		r=chdir(args[1]);
		//      printf("%d\n",r);
		//              getcwd(currd, sizeof(currd));
		if (r!=0)
			perror("Could not enter directory\n");
	}
	return 1;
}
int s_pinfo(char ** args)
{
	FILE *f;
	char b1[256],b2[256],name[256],s;
	long unsigned int size;
	int pid;
	if(args[1]==NULL)
	{
		sprintf(b1,"/proc/%d/stat",getpid());
		sprintf(b2,"/proc/%d/exe",getpid());
	}
	else
	{
		sprintf(b1,"/proc/%s/stat",args[1]);
		sprintf(b2,"/proc/%s/exe",args[1]);
	}
	if((f = fopen(b1,"r"))!=NULL)
	{
		fscanf(f,"%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d",&pid, &s, &size);
		fclose(f);									
		printf("pid -- %d\nProcess Status -- %c\nmemory -- %lu\n",pid,s,size);
		readlink(b2,name,256);
		printf("Executable Path -- %s\n",name);
	}
	else
		perror("No such process running");

	return 1;
}
int s_listjobs(char ** args)
{
	int s,ppid,count;

		while((ppid = waitpid(-1, &s, WNOHANG)) > 0) {
      //  printf("PROCESS WITH PID-%d EXITED NORMALLY\n",ppid);
    	node* temp=head;
    	node* ptemp;
    		
    		if(temp!=NULL && temp->proid == ppid)
    		{
    				if(s==0)
    				printf("PROCESS %s WITH PID-%d EXITED NORMALLY\n",temp->cargs[0],ppid);
    				else
    					printf("PROCESS %s WITH PID-%d WAS TERMINATED\n",temp->cargs[0],ppid);
  				head = temp->next;
    			free(temp);
    			if(head == NULL)
    				tail=NULL;
    		}
    		else
    		{
    			while(temp != NULL && temp->proid != ppid)
    			{
    				ptemp =temp;
    				temp=temp->next;
    			}
    			if (temp==NULL)
    			{
    				tail=ptemp;
    			}
    			else if(temp!=NULL)
    			{
    				if(s==0)
    				printf("PROCESS %s WITH PID-%d EXITED NORMALLY\n",temp->cargs[0],ppid);
    				else
    					printf("PROCESS %s WITH PID-%d WAS TERMINATED\n",temp->cargs[0],ppid);
    				ptemp->next=temp->next;
    				if(temp->next==NULL)
    					tail=ptemp;
    			}
    			free(temp);
    	}
}
	if(head==NULL)
		printf("No background jobs running at present\n");
	else
	{
		node * temp=head;
		int pin=1;
		while(temp!=NULL)
		{
			printf("[%d]",pin);
			count=0;

			while(temp->cargs[count]!=NULL)
			{
				printf("%s ", temp->cargs[count]);
				count++;
			}
			printf("[%d]\n",temp->proid);
			temp=temp->next;
			pin++;
		}
		printf("Are the current bg processes\n");
	}
	return 1;

}
int s_fg(char ** args)
{
if(args[1]==NULL)
{
	printf("Use fg <job id>\n");
	return 1;
}
	int pnum=atoi(args[1]);
	int i, kid;
node* temp=head;
node* ptemp;
char ** fargs; 
    		
	if(temp!=NULL && pnum==1)
    {
    	fargs=temp->cargs;
    	kid=temp->proid;
  		head = temp->next;
    	free(temp);
    	if(head == NULL)
    		tail=NULL;
    }
    else
    		{
    			i=1;
    			while(temp != NULL && pnum>i)
    			{
    				ptemp =temp;
    				temp=temp->next;
    				i++;
    			}
    			if (temp==NULL)
    			{
    				printf("No such job number\n");
    				tail=ptemp;
    				return 1;
    			}
    			else if(temp!=NULL)
    			{
    				fargs=temp->cargs;
    				kid=temp->proid;
    				ptemp->next=temp->next;
    			}
    			free(temp);
    	}
    	int s;
    	waitpid(kid, &s, 0);
}
int s_sig(char ** args)
{
if(args[1]==NULL || args[2]==NULL)
{
	printf("Use kjob <job id> <signal no.>\n");
	return 1;
}
int pnum=atoi(args[1]);
node * temp=head;
int i=1;
while(temp != NULL)
{
	if(i==pnum)
		{
			kill(temp->proid, atoi(args[2]));
			printf("Killed\n");

			break;
		}
	i++;
	temp=temp->next;
	perror("Done killing");
}
if(temp == NULL)
	printf("No such process number\n");
return 1;
}

int s_killallbg()
{
	node * temp=head;
	while(temp != NULL)
	{
			kill(temp->proid, 9);
			printf("Killed\n");
			temp=temp->next;
	}
return 1;
}
int s_exec(char ** args)
{    

	if(args[0]==NULL)
		return 1;
	else if(pipflag==1)
		s_pother(args);
		else if(strcmp(args[0],"jobs")==0)
	{       
		s_listjobs(args);
	}
	else if(strcmp(args[0],"fg")==0)
	{       
		s_fg(args);
	}
	else if(strcmp(args[0],"kjob")==0)
	{       
		s_sig(args);
	}
	else if(strcmp(args[0],"killallbg")==0)
	{       
		s_killallbg();
	}
	else if(strcmp(args[0],"cd")==0)
	{       
		s_cd(args);
	}

	else if(strcmp(args[0],"pwd")==0)
		s_pwd();
	else if(strcmp(args[0],"echo")==0)
	{       
		s_echo(args);
	}
	else if (strcmp(args[0], "pinfo")==0)
		s_pinfo(args);
	else if (strcmp(args[0],"quit")==0 || strcmp(args[0],"exit")==0)
		s_exit();
	else if (strcmp(args[0],"help")==0)
		s_help();
	else
			s_other(args);
		
}

int s_redirect(char ** args)
{
	int sout = dup(1);
	int s_in = dup(0);
	int fd1,fd2,fd3,sl;
	 if (inflag == 1)
	{
		inflag=0;
		fd1 = open(infile,0644);
		if(fd1 == -1){
			perror("Failed to open file");
		}
		close(0);
		if(dup2(fd1, 0) != 0)  // 0 refers to stdin
			perror("dup2 for write fail");

		close(fd1);
		

	
	}
	if(outflag==1)
	{
		fd2 = creat(outfile,0644);
		if(fd2 == -1){
			perror("Failed to create file");
		}
		close(1);
		if(dup2(fd2, 1) != 1)  // 1 refers to stdout
			perror("dup2  for read fail");

		close(fd2);

		outflag=0;

	}

 if(appflag==1)
	{
		fd3 = open(appfile,O_WRONLY|O_APPEND|O_CREAT,0644);
		if(fd3 == -1){
			perror("Failed to open file");
		}
		close(1);
		if(dup2(fd3, 1) == -1)  // 1 refers to stdout
			perror("dup2  for APPEND fail");
		close(fd3);
		appflag=0;
		
	}
		sl= s_exec(args);
		dup2(s_in, 0);
		dup2(sout, 1);
		return sl;

}

int s_pexec(char** pparts,char ** args,int i,int num,int pipefd[] )
{
	int pid1;
	int l; 
	//perror("Entered");
	pid1=fork();
	if (pid1 < 0) 
	{
		perror("Could not create process");
		exit(1);
	}

	else if(pid1==0) // child process
	{
		if(i==0)
		{
		
		if(dup2(pipefd[1], 1)<0)
      	{
      		perror("dup21");
      		exit(EXIT_FAILURE);
      	}
		}

		else if(pparts[i+1]!=NULL)//middle
		{
		if(dup2(pipefd[(2*i)-2], 0)<0)
      	{
      		perror("dup22");
      		exit(EXIT_FAILURE);
      	}
		
      	if(dup2(pipefd[2*i+1], 1)<0)
      	{
      		perror("dup23");
      		exit(EXIT_FAILURE);
      	}
      	}

		else if(pparts[i+1]==NULL)//last		{            
    	{
    	if(dup2(pipefd[(2*i)-2], 0)<0)
    	{
    		perror("dup24");
      		exit(EXIT_FAILURE);

    	}            
   		}

   		for(l=0;l<2*num;l++)
   		{
   	//		perror("Closed");
   			close(pipefd[l]);
   		}
   		pipflag=1;
		s_redirect(args);


	}

 
		
return 1;
}



