int s_other(char ** args);
int s_exit();
int s_pwd();
int s_help();
int s_pinfo();
int s_echo(char ** args);
int s_cd(char ** args);
int s_exec(char ** args);
int s_redirect(char ** args);
char * s_read();
char ** s_tokenize(char * line);
char ** s_tokenize2(char * line);
char ** s_ptokenize(char * line);
int s_pexec(char** pparts,char ** args,int i, int num,int pipefd[] );
int s_listjobs(char ** args);
int s_fg(char ** args);
int s_sig(char ** args);
int s_killallbg();
void  SIGINT_handler(int sig);