#include"myheader.h"

	
char *tokenArray[50];
char file_path[50];


//----------------------------------------------MAIN FUNCTION--------------------------------------------||
int main()
{  
	int status=-1;
	/*
	 *  creating a file for storing history of file
	 *
	 */
	strcpy(file_path , (char*)get_current_dir_name());
	strcat(file_path ,"/history.txt");
	FILE *fp=fopen(file_path ,"a+");
	

	system("clear");
	while(TRUE)
		{
			printf("new@shell : %s$",(char*)get_current_dir_name());
			
			char command[50],ch,temp[50] ,redirect[50];
			int i=0,k=0;
			while(( ch=getchar()  )!='\n')			//take input until user press enter
			    command[i++]=ch;
			    command[i]='\0';

			fprintf(fp,"%s\n" ,command);
			strcpy(temp , command);
						
			parse(temp,tokenArray ," \t\n");		//calling parsing funcion defined in parser.c
			
			if(tokenArray[0]==NULL)				//user press Enter
			   continue;
			else if(is_pipe(command))		//If command contain | operator, execute pipe commands
        		   	pipe_command_execute(command);	
			   		
			else if(is_io_redirect(command))
				{			
				if(!fork())
			   	handle_io_redirect(command);
				else
				wait(&status);
				}
			else if(is_redirect(command))			
			   	redirect_command_execute(strcpy(redirect,command) , (int *)set_redirect_flag(command));
				
//***********************************some internal commands*********************************************
			else if(strcmp(tokenArray[0],"cd")==0)			//check for CD command
			   	change_Directory();
   			else if(strcmp(tokenArray[0],"echo")==0)	//check for ECHO command
        		   	exe_echo(tokenArray);
			else if(strcmp(tokenArray[0],"wc")==0)
   			  	handle_wc(tokenArray);
			else if(strcmp(tokenArray[0],"pwd")==0)		//check for PWD command
   			   		exe_PWD();
   			else if(strcmp(tokenArray[0],"exit")==0)	//execute exit command
        		    {
        		    	fclose(fp);
        		        system("exit");
        		    }
   			else if(strcmp(tokenArray[0],"clear")==0)	//execute clear command
   			        system("clear");
            		else if(strcmp(tokenArray[0],"help")==0)	//display commands implemented by shell
   		  	        do_help();
   		  	else if(strcmp(tokenArray[0],"history")==0)
   		  			{
   		  				fclose(fp);
						print_history();
						fp=fopen(file_path ,"a+");
   		  			}        
   			else
   			        command_ALL(tokenArray);

   		tokenArray[0]=NULL;
	}
return 0;
}

//*******************************************************************************
//**********************TO IMPLEMENT CD COMMAND********************************
void change_Directory()
{
	
        if (tokenArray[1] == NULL || strcmp(tokenArray[1],"~")==0 ) 
		{
               		chdir(getenv("HOME"));
		}                                             
        else if (   chdir(tokenArray[1] ) == -1   )
		{                                  //unsuccessful chdir returns -1
                printf(" %s: no such directory\n", tokenArray[1] );
		}        
}

//*****************************************************************************
//**********************TO IMPLEMENT HISTORY COMMAND********************************

void print_history()
{
		char line[30] ;
		int cnt=0 ,i=0;
		FILE *fp=fopen(file_path , "r");
		while(fgets(line ,100 , fp))
			printf("%s" ,line);
   	
   		 
}
//****************************************************************
//******************TO FIND LENGTH OF ANY STRING******************
int len(char *p)
	{
		int i;
		for(i=0 ; *(p+i)!=0x0 ;i++);
		return i;
	}
//*****************************************************************
//**********TO CHECK WHETHER COMMAND CONTAIN PIPE OR NOT***********
int is_pipe(char *p)
	{
		int i;
		for(i=0; i< len(p)-1 ;i++)
			if(*(p+i)=='|')
				return TRUE;
			return FALSE;
	}


//******************************************************************
//**********TO CHECK WHETHER COMMAND CONTAIN ">" "<" OR NOT***********

int is_redirect(char *p)
	{
		int i;
		for(i=0; i< len(p)-1 ;i++)
			if(*(p+i)=='>' || *(p+i)=='<')
				return TRUE;
			return FALSE;
	}

int is_io_redirect(char *p)
	{
		if (strstr(p ,"<") && strstr(p,">"))
		{
		printf("io");
		return TRUE;
		}
		else
		return FALSE;
		
	}

//*******************************************************************
////**********TO HANDLE REDIRECTION***********

int set_redirect_flag(char *p)
	{
	
		int *flag=(int *)malloc(sizeof(int)*3), i=0;
								//flag[0]=Input redirection
								//flag[1]=Output Redirection ">" overwrite Mode
								//flag[2]=Output redirection ">>" Append Mode

		while(i<3)
			flag[i++]=0;
		char *ptr1,*ptr2,*ptr3;
		ptr1=strstr(p,">");  //getting first occurence of ">" if exist then reset flag
		if(ptr1!=NULL)           
        	flag[0]=0;
   		else
      		for(i=0;i<strlen(p)-1;i++)
           			if(*(p+i)=='<')
            		{
            		 	flag[0]=1;
               			if(*(p+i+1)=='<')
               				{ 
                 			flag[0]=0;      	//reset flag to show error in "<<"
                 			break;
               				}
             		}
         	ptr1=strstr(p,"<");
		if(ptr1!=NULL)
        	  flag[1]=0;
   		else
      		  for(i=0;i<strlen(p)-1;i++)
           	      if(*(p+i)=='>')
            		{
            		 flag[1]=1;
             		 if(*(p+i+1)=='>')
               		   {
                 	    flag[1]=0;
			    flag[2]=1;
                 	    break;
     	           	   }
        		 } 
        	 
return (int )flag;
}

//****************************END OF FILE********************************
