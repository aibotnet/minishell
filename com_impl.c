//********************THIS FILE CONTAINS IMPLEMENTATION OF INTERNAL COMMANDS*****************
#include "myheader.h"
char *part[50];

int no_of_word;
int no_of_char;
int no_of_line;

char** parser(char *temp)
	{
		int i,n=1;
		char **toke;
  		toke=(char**)malloc(sizeof(char*)*20);
  		for(i=0;i<20;i++)
  		toke[i]=(char*)malloc(sizeof(char)*20);
  		toke[0] =(char *)strtok(temp," ");
 		if(toke[0]==NULL)
     			return toke;
  		while((toke[n] =(char *) strtok(NULL ," ")) != NULL)
     		n++;
  		toke[n] = NULL;
		return toke;
	}
//********************************************************************************************
//***************FUNCTION TO EXECUTE ALL EXTERNAL COMMANDS************************************


void command_ALL(char **command){
		int pid=fork();
		if(pid==-1)
		      printf("fork fail!!!!!!!!");
		if(!pid)
		   if(execvp(*command,command)== -1)
		      printf("%s  : command not found	\n",*command);
		if(pid)
		   wait(100);
	}

void count(char* filename){
		no_of_word=0;
		no_of_char=0;
		no_of_line=0;
		char line[50];
		int i=0 ,nc=0,nw=0,nl=0;
		FILE *fp=fopen(filename , "r");
		if(!fp)
			printf("UNABLE TO OPEN FILE");
		else
		while(fgets(line,100 , fp))
			{
				i=0;
				while(*(line+i))
				{
					if(*(line+i)==' '||*(line+i)=='\n' )
						if ( i>0 && (*(line+i-1)!=' ') )
					 		nw++;
					i++;
				}
				nc+=strlen(line)-1;
				nl++;
			}
		 no_of_word+=nw;
		 no_of_char+=nc;
		 no_of_line+=nl;
		printf("\n%d\t%d\t%d  %s",nc , nw ,nl,filename);
			
	}
//*********************************************************************************************
//********************FUNCTION TO HANDLE WORD COUNT COMMAND**********************************

void handle_wc(char** token)
{
	int i=1 , filecount=0 ,op_count=0;
	while(token[i])
	{
		if(strstr(token[i] ,"-"))
		op_count++;
		else
		count(strtok(token[i] , " "));
		i++;
	}
	if(op_count==3)
		printf("\ntotal :-\n%d\t%d\t%d\n ",no_of_char,no_of_word,no_of_line);
	else if(op_count==2)
		{
			if( (strcmp(token[1],"-l")==0  ||  strcmp(token[2],"-c")==0) && (strcmp(token[1],"-l")==0  ||  strcmp(token[2],"-c")==0))
			printf("\ntotal :-charecter= %d line= %d\n",no_of_char ,no_of_line);
			else if( (strcmp(token[1],"-l")==0  ||  strcmp(token[2],"-w")==0) && (strcmp(token[1],"-w")==0  ||  strcmp(token[2],"-l")==0))
			printf("\ntotal :-word= %d line= %d\n",no_of_word ,no_of_line);
			else if( (strcmp(token[1],"-c")==0  ||  strcmp(token[2],"-w")==0) && (strcmp(token[1],"-w")==0  ||  strcmp(token[2],"-c")==0))
			printf("\ntotal :-charecter= %d word= %d\n",no_of_char ,no_of_word);
		}
	else if(op_count==1)
		{
			if( strcmp(token[1],"-l")==0)
			printf("\ntotal :-line= %d\n",no_of_line);
			else if( strcmp(token[1],"-c")==0)
			printf("\ntotal :-charecter= %d\n",no_of_char);
			else if(strcmp(token[1],"-w")==0)
			printf("\ntotal :-word= %d\n",no_of_word);
			else if(strcmp(token[1],"-lc")==0)
			printf("\ntotal :-charecter= %d line= %d\n",no_of_char ,no_of_line);
			else if(strcmp(token[1],"-lw")==0)
			printf("\ntotal :-word= %d line= %d\n",no_of_word ,no_of_line);			
			else if(strcmp(token[1],"-cw")==0)
			printf("\ntotal :-charecter= %d word= %d\n",no_of_char ,no_of_word);
			
		}
	else
		printf("\ntotal :-\n%d\t%d\t%d\n ",no_of_char,no_of_word,no_of_line);
}

//*********************************************************************************************
//********************FUNCTION TO HANDLE REDIRECTION OPERATOR**********************************
void handle_redirect(char* command , int flag)
	{  
			
    		pid_t pid;                
    		int i=0,status=1;
    		char *arg[50];
    		char *part[50];
      		if(flag==1)
      		{           
   				parse(command, part ,"<"); 
				parse(part[0], arg ," "); 
  		    		pid=fork();
   		    		if(pid<0)             
      					printf("error in forking!!!!!!");
   		    		else if(0==pid)
	   				{
	   			 		char* filename=strtok(part[1] , " ");
    			 			if (!dup2( open(filename,O_RDONLY),0))        
       			    			command_ALL(arg);
       			 			else
       			    			printf("unable to open file!!!\n");       
   			 		}
   		     		else
      					wait(&status);
      		  }
      		  else if(flag==2)
      		  {
      		    		 part[0]=strtok(command,">");    
   		     			while( part[++i]=strtok(NULL ,">") );           
   						parse(part[0],arg ," "); 
   			
	           	  	pid=fork();
   		      			if(pid<0)             
      						printf("error in forking!!!!!!");
   		      			else if(0==pid)
   							{
   			   				char* filename=strtok(part[i-1] , " ");
   			   				dup2(open(filename , O_CREAT  | O_WRONLY | O_TRUNC,0777) , 1);
    			   				command_ALL(arg);	
			   				exit(0);    
   			 				}
   		       				else
      			  				wait(&status);
      		     }
      		    else
      		     {
      					part[0]=strtok(command,">>");    
   						while( part[++i]=strtok(NULL ,">>") );             
						parse(part[0],arg ," "); 
						pid=fork();
						if(pid<0)             
      			  			printf("error in forking!!!!!!");
   						else if(0==pid)
   			 				{
   			  				char* filename=strtok(part[i-1] , " ");
   			  				dup2(open(filename,O_CREAT | O_APPEND |O_WRONLY,0777) , 1);
    			  			command_ALL(arg);
			  				exit(0);    
   			 				}
   							else
      			   				wait(&status);		
      		       }
	}
	
	
	
	
	
	

void handle_io_redirect(char* args)
{
	printf("hello");
		char l[30],r[30],m[30],**toke1,**toke2,**toke3;
  		int i=0,j=0,k=0;
 	        char *temp,ch;
		temp=args; 
	        int pid,status,fd1,fd2,fd3;
  		int count1=0,count2=0,count21=0,count12=0;
    //.......code for checking type of redirection..........//
  		while(*temp!='\0')
  			{
    
    				if(*temp=='>')
      					{ 
        					count1++; //...count1 is to count ( >).....//
        					temp++;
        					if(*temp=='>')
          						{
								count1++; //..... count1 is incremented to check if ( >>).....//
								break;
	 						}
                                                while(*temp!='\0')
						{
                                                  if(*temp=='<')
							{
                                                   	  count12++;  //... count12 is to check if  ( > <).....//
                                                          count1=0;   //......count1 is reset to 0....if (> <)....//
                                                          break;
							}   
						   else	
                                                      temp++;
						}
                                              break;	    
      					}

     				if(*temp=='<')
      					{
       						count2++;  //...count2 is to check if ( <).....//
                                                temp++;
                                               while(*temp!='\0')
						{
                                                  if(*temp=='>')
							{
                                                   	  count21++;  //... count21 is to check if  ( > <).....//
                                                          count2=0;   //......count2 is reset to 0....if ( < >)....//
                                                          break;
							}   
						   else	
                                                      temp++;
						} 
       						break;
      					}
   				else
     					temp++;
  			}
                 //.........end.....//
            
     //.....parser for input and output(>,<) redirection.......//
		if(count1==1||count1==2)
          		ch='>';
        if(count2==1)
          		ch='<';

                if(count1==1||count1==2||count2==1)
				{
	 	   while( *args!=ch)
	    		{
	       			l[i]=*args;
	       			args++;
	       			i++;
	    		}
	   		l[i]='\0';
	   		toke1=parser(l);//....parsing left string part before (<) or (>) and storing in pointer toke1......//
	
	  		args++;
        	   	if(*args=='>')
        		   args++;
     	

			 while(*args!='\0')
				  {
				    r[j]=*args;
				    args++;
				    j++;
				  }
			  r[j]='\0';
			  toke2=parser(r);//....parsing right string part after (<) or (>) and storing in pointer toke2......//
		 }
           //........end.........//

   //.....parser for input&output(<>,><) redirection.......//               
               if(count21==1||count12==1)
	               {
                          if(count12==1)
          		     ch='>';
        	          if(count21==1)
          		       ch='<';       
                          while( *args!=ch)
	    		      {
	       		         l[i]=*args;
	       			 args++;
	       		         i++;
	    		      }
	   		l[i]='\0';
	   		toke1=parser(l);//....parsing left string part before (< >) or (> <) and storing in pointer toke1......//
	
	  		args++;
     	                 if(count12==1)
          		     ch='<';
        	          if(count21==1)
          		       ch='>';

			 while(*args!=ch)
				  {
				    m[j]=*args;
				    args++;
				    j++;
				  }
			  	m[j]='\0';
			  	toke2=parser(m);//.parsing string part in between (< string >) or (>string<) and storing in pointer toke1.//
                          	args++;
                          	while(*args!='\0')
					  {
					    r[k]=*args;
					    args++;
					    k++;
					  }
			  	r[k]='\0';
			  	toke3=parser(r);//....parsing right string part after (< >) or (> <) and storing in pointer toke3......//
			}
            //...........ends........//

  	//////---------code for output redirection(>)..............//
     			  if(count1==1)
     				{
					fd1=open(toke2[0],O_CREAT|O_WRONLY|O_TRUNC,0777);
      					close(1);
        				dup(fd1);
        				close(fd1);
       					execvp(*toke1,toke1);
     
      				}	
                   //...........end........//

         //////---------code for output redirection in append mode(>>)..............//
			 if(count1==2)
			      {
                                     	fd2=open(toke2[0],O_CREAT | O_APPEND |O_WRONLY,0777);
					close(1);
					dup(fd2);
					close(fd2);
					execvp(*toke1,toke1);
		
			     }
                    //...........end........//

       //////---------code for input redirection(<)..............//
			if(count2==1)
				{
	                                
					fd2=open(toke2[0],O_RDONLY,0777);
					close(0);
					dup(fd2);
					close(fd2);
					execvp(*toke1,toke1);
				}
                    //...........end........//

        //////---------code for input&output redirection(< >)..............//
                        if(count21==1)
                                {
                                   		
                                     	fd2=open(toke2[0],O_RDONLY,0777);
                                        fd3=open(toke3[0],O_CREAT|O_WRONLY|O_TRUNC,0777);
					close(0);
					dup(fd2);
					close(fd2);
                                       
                                        dup2(fd3,1);
                                        close(fd3);
					execvp(*toke1,toke1);
				      
			       }
                   //...........end........//

        //////---------code for input&output redirection(><)..............//
                       if(count12==1)
                                {
                                   		
                                     	fd2=open(toke3[0],O_RDONLY,0777);
                                        fd3=open(toke2[0],O_CREAT|O_WRONLY|O_TRUNC,0777);
					close(0);
					dup(fd2);
					close(fd2);
                                       
                                        dup2(fd3,1);
                                        close(fd3);
					execvp(*toke1,toke1);
				      
			       }
                   //...........end........//
}		
//**********************************************************************************************
//************************FUNCTION TO EXECUTE REDIRECT COMMAND**********************************
void redirect_command_execute(char* command , int *flag)
	{
	   if(*flag && !(*(flag+1)) && !(*(flag+2)) )
	     handle_redirect(command ,1);	
	   else if(!*flag && *(flag+1) && !(*(flag+2)) )
	     handle_redirect(command , 2);
	   else if(!(*flag) && !(*(flag+1)) && *(flag+2) )
	     handle_redirect(command , 3);
	   else 
	     printf("invalid redirection!!!");		
	}

//************************************************************************************************
//**************************FUNCTION TO EXECUTE PIPES*********************************************
void pipe_command_execute(char* command)
	{
			char *commandArray1[50];	
			char *commandArray2[50];
			char *token[20] , *ch;
			int pid1 , pid2 , flag=0 , i=0;
				parse(command,token,"|");	
				parse(token[0],commandArray1 ," ");
				parse(token[1],commandArray2 ," ");
			 pid1=fork();			//To execute command1
   if(pid1 < 0)
   {
        printf("error in forking!!!!!!");
        exit(1);
   }
   else if(!pid1)
   {
    			  int fd_array[2]; 
                          if(pipe(fd_array) == -1)		//pipe() called. It takes integer array of size 2 as 									//parameter. 
                          		printf("error in pipe system call");                        		 
                            pid2   = fork();
                            if(pid2<0)
                            {
                                printf("error in forking!!!!!!");
                                exit(1);             
                            }
                            else if (pid2==0)		//output goes to pipe instead of standard output
                            {	
                                dup2(fd_array[1] ,1);  //dup2() duplicates file descriptor fd_array[1] with standard output
                                close(fd_array[1]);    //closing file desciptors 
                                close(fd_array[0]);                                             
                                command_ALL(commandArray1); 
                            }
                            else				//reading from pipe instead of standard input 
                            {
			       dup2(fd_array[0] ,0);   //dup2() duplicates file descriptor fd_array[0] with standard input
                               close(fd_array[0]);     //closing file descriptors
                               close(fd_array[1]);  
                               command_ALL(commandArray2); 
                            }
		exit(0);     
     }    
     else
         wait(&flag);   				//parent process is waiting for the child processes to finish 								//execution.      
}

//***********************************************************************************************
//***************************FUNCTION TO EXECUTE ECHO COMMAND************************************
void exe_echo(char *args[])
{
    int i=1;
for(;i<len(args);i++)
	{
    if(args[i]==NULL)
	{
        printf("\n");
	break;	
	}

    else if(strcmp(args[i],"$PWD")==0 )
       printf("%s ",(char *)getenv("PWD"));
    else if(strcmp(args[i],"$HOME")==0)
       printf("%s ",(char *)getenv("HOME"));
    else if(strcmp(args[i],"$OLDPWD")==0)
       printf("%s ",(char *)getenv("OLDPWD"));
    else if(strcmp(args[i],"$PATH")==0)
       printf("%s ",(char *)getenv("PATH"));
    else if(strcmp(args[i],"$USER")==0)
       printf("%s ",(char *)getenv("USER"));
   
	}
}

//***************************************************************************************************
//*************************FUNCTION TO EXECUTE PWD COMMAND*******************************************
void exe_PWD()
{
   char *ch;
   ch=(char *)getenv("PWD");
   if(ch==NULL)
     printf("invalid command!!!\n");
   else
   printf("%s\n",ch);
}


//****************************************END OF FILE****************************************************
