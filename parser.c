//*********************THIS F+ILE CONTAINS PARSER FUNCTION****************************
#include"myheader.h"
void parse(char *str, char *arg[] , char *del){
	char *ch;
	int i=0,flag=0;
	ch=strtok(str,del);
		while(ch!=NULL){
			arg[i++]=ch;
			flag=1;
			ch=strtok(NULL,del);
			}
		if(!flag)
			i++;
		arg[i]=NULL;
}
