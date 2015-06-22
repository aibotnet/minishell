				/*in this file we write all header file 
				 *
				 *and all function prototype
				 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>


#define TRUE 1
#define FALSE 0




//function prototypes
void parse(char * , char** , char*);
void change_Directory();
void do_help();
void command_ALL(char **);
int  is_pipe();
int  is_redirect();
int  set_redirect_flag(char *);
void pipe_command_execute(char*);
void handle_redirect(char*, int );		
void redirect_command_execute(char* , int *);
void exe_echo(char **);
void exe_PWD();
void exe_wc(char *,char *);
void print_history();
void handle_io_redirect(char *);
