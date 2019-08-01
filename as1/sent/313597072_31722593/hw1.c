/******HW1 - operating systems,spring 2019*******/
/*****************Sivan and Vlad*****************/

/*Libraries to be included*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/*function declarations*/
void errorMSG(char* msg);
void shell();
void shell1();
void shell2();
void shell3();
void readFile(char* fName);


void main()
{
	shell();
}

/*First shell. Supports ps system function via "jobs" command, 
	exiting the shell through "by" command.*/
void shell1()
{
	int id;	// for process controll
	char buffer[100];	// for user input
	
	do
	{
		printf("shell1$** ");	// cursor
		scanf("%s", buffer);
		
		if (strcmp("by", buffer) == 0) continue;
		else if (strcmp("jobs",buffer) == 0)
		{
			if( ( id =  fork() ) == 0 )	execlp("ps","ps",(char *)0);	// execute if child
			else	waitpid(id,NULL,0);	// wait for child if parent
		}
		
		else perror("The function you've entered is not supported. please try again with supported functions (jobs or by).\n");
	}
	while (strcmp("by",buffer) != 0);
	
		// return to primary shell if user choses to exit
}

/*Second shell. Supports date system function via "mydate" command, 
	exiting the shell through "by" command.*/
void shell2()
{
	int id;	// for process controll
	char buffer[100];	// for user input
	
	do
	{
		printf("shell2$** ");	// cursor	
		scanf("%s", buffer);
		
		if (strcmp("by", buffer) == 0) continue;
		else if (strcmp("mydate", buffer)==0)
		{
			
			if( ( id =  fork() ) == 0 )	execlp("date","date",(char *)0);	// execute if child
			else	waitpid(id,NULL,0);	// wait for child if parent
		}
		
		else perror("The function you've entered is not supported. please try again with supported functions (mydate or by).\n");
	}
	while (strcmp("by", buffer) != 0);
	
	// return to primary shell if user choses to exit
}

/*The third shell. Executes system commands as written in a given file via "readFile" command,
	exiting the shell through "by" command.*/
void shell3()
{
	char buffer[100], fn[100];	// for user input and function nevigation respectively
	char *token;	// for file name
	fgets(buffer, 100, stdin);	// Takes care of "\n" caused by entering to the shell
	
	do
	{
		printf("shell3$** ");	// cursor		
		fgets(buffer, 100, stdin);	// take input from user
		
		if (strcmp("by\n", buffer) == 0) continue;
		else
		{
				token = strtok(buffer, " ");	// trim user input in spaces				
				
				strcpy(fn, buffer);	// copy in order to change only for command checking
				fn[8] = '\0';
				if(strcmp("readFile", fn) == 0)	
				{
					token = strtok(NULL, " \n");	// move to next token
					readFile(token);	// call function to read inputed file
				}
		}
	}
	while (strcmp("by\n",buffer) != 0);
	
		// return to primary shell if user choses to exit
}

/* A function for reading a file and executing system commands from it.
	Assumes syntax of the file to be valid. 
	Cannot execute a function that has more than 3 arguments.*/
void readFile(char* fName)
{
	int id, fd, r, l,commandl, i=0, sumcommandl = 0; 	// for process id, file descriptor, byetes read in he file, length of file, length of line in the file, index, amount of lines that have been executed accordingly.
	char buffer[999], *curbuf,*nextbuf, *arg[4];	// file text in a string, current command, next command , arguments to execute accordingly.
	fd = open(fName, O_RDONLY);
	if(fd <= 0)errorMSG("There seems to be a problem with opening the file you've entered. Exiting now.\n");
	
	l = lseek(fd, 0, SEEK_END);	// find length of data within the file
	lseek(fd, 0, 0);
	
	if(r=read(fd, buffer, l)>0)
	{
		close(fd);
		buffer[l] = '\n';	//	becuase we cut by detecting \n
		buffer[l+1] = '\0';	// close the string
		curbuf = buffer;	//	
		nextbuf = strchr(curbuf,'\n') +1;
		commandl = nextbuf - curbuf;	//location of 
		
		while(sumcommandl < l-1){	//exit by no command param and we know last \n is in l
			char command[100],*varcommand;
			strncpy(command,curbuf,commandl);
			command[commandl-1] = '\0';
			varcommand =strtok(command," \n");
			i=0;
			
			if(varcommand == NULL)break;	//may enter with no command but the varcommand will be null
			while(varcommand!=NULL && i<4){
				arg[i] = varcommand;
				varcommand=strtok(NULL," \n");
				++i;
			}
			arg[i] = NULL;
			
			
			if( ( id =  fork() ) == 0 )		// execute if child
				{
						execvp(arg[0], arg);
						errorMSG("Error when reading file: problem with the current function. To your information, the shell cannot recieve more than 3 arguments.\n");	//in case of error not supose to enter here	
				}else{
						 waitpid(id,NULL,0);	// wait for child if parent
				 }

			curbuf = nextbuf;
			nextbuf = strchr(curbuf,'\n') +1;
			commandl = nextbuf - curbuf;	//location of
			sumcommandl +=commandl;
		}
	}
	
	return;	// return to shell3
}
		

/* Primary shell. Supports entering the 3 other shells and exiting the program via command "exit".*/
void shell()
{
	int id;	// for process controll
	char buffer[100];	// for user input
	
	do
	{
		printf("shell$** ");	// cursor
		scanf("%s", buffer);
		
		if (strcmp("exit",buffer)==0) exit(0);
		else
		{
			if(strcmp("shell1",buffer)==0) shell1();
			else if(strcmp("shell2",buffer)==0) shell2();
			else if(strcmp("shell3",buffer)==0) shell3();
			else	printf("The function you've entered is not supported in this shell. Please try again\n");	
		}
	}
	while(strcmp("exit",buffer)!=0);
	
	exit(0);	// exit successfully
}

/*Function fo exceptions handling. Prints the inputed error massage and breaks frop program*/
void errorMSG(char* msg)
{
	perror(msg);
	exit(2);
}
