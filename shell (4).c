#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
struct node
{
	struct node* next;
	char *data;
};

	struct node *head;
	struct node *current; 

void insert(char *list){
	if(head== NULL){
		struct node* newnode= (struct node*) malloc (sizeof(struct node)); 
		newnode-> data= list; 
          newnode->next= NULL; 
		head= newnode; 
		current= newnode; 
	}
	else {
	struct node* newnode= (struct node*) malloc (sizeof(struct node));
	struct node* temp= head;
	//temp= head;
	while(temp->next!= NULL){
	temp= temp->next; 
	}
	newnode->next=NULL; 
     newnode->data= list;
     temp->next= newnode; 
	current= newnode; 
}
}

void printlist(){
     struct node* temp= head;
     while(temp!= NULL){
          printf("%s \n", temp->data); 
          temp = temp->next;
     }
     printf("\n"); 

}

void printcommandlist(){
     printf("\nCommands: \n");
     struct node* temp= head; 
     for(struct node* temp=head; temp != NULL; temp=temp->next) {
          if(temp == head) {
               puts(temp->data);
          } else if(strcmp(temp->data, "|") == 0) {
               puts(temp->next->data);
          }
     }
     // while(temp!= NULL || temp->data != '<'||temp->data != '>'||temp->data != '&'||temp->data != '|'){
     //      temp= temp->next;
     // }
     // if (temp->data != '<'||temp->data != '>'||temp->data != '&'||temp->data != '|'){
     //      printf("%s \n", temp->next->data); 
     // }
}

void printcomarg(){
    printf("\nCommands with Args: \n");
     struct node* temp= head; 
     struct node* temp2; 
     for(struct node* temp=head; temp != NULL;) {
          if(temp == head) {
               puts(temp->data);
               temp=temp->next;
               while(temp != NULL && (strcmp(temp->data, "|")!=0)) {
                    printf("%s ", temp->data);
                    temp=temp->next;
               }
               printf("\n\n");
          } else if(strcmp(temp->data, "|") == 0) {
               puts(temp->next->data); //grep
               temp = temp->next->next; //| -> grep -> world
               while(temp != NULL) {
                    printf("%s ", temp->data);
                    temp=temp->next;
               }
               printf("\n");
          }
     }
     printf("\n");
}


void delete(){
     
     while (head-> next != NULL){
          struct node* temp= head; 
          head= head->next; 
          free(temp); 
     }
     free (head); 
}

void  parse(char *line, char **argv)
{ 
     int quote=0; 
	 
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == '\t' || *line == '\n'|| *line== ' '){
                   /* replace white spaces with 0    */
               *line++= '\0';
          }
          if(*line == '"') {
               *argv++ = ++line;
               while(*++line != '"');
               *line++ = '\0';
          } else {
               *argv++ = line;          /* save the argument position     */
               while (*line != '\0' && *line != ' ' && 
                    *line != '\t' && *line != '\n') {
                    line++;
               }
          }
                            /* skip the argument until ...    */
     
     }
     *(--argv) = NULL;                 /* mark the end of argument list  */
}

void  execute(char **argv) //write your code here
{
     char argv2[64]; 
     
//HINT you need to introduce the following functions: fork, execvp, waitpid
//Advanced HINT: pipe, dup2, close, open

     pid_t  pid, pid1,pid2;
     int    status;
     int fd, fd_temp; 
     for(int i=0; argv[i] != NULL; i++){
          insert(argv[i]);
     }

     printlist();
     printcommandlist();
     printcomarg(); 
     delete(); 
     //printf("Printing list after deleting\n"); 
     //printlist(); 

     int count=0; 
     int countf=0; 
     int num=0; 

     for(int i=0; argv[i] != NULL; i++){
          if(*argv[i] == '>' || *argv[i] =='<'||*argv[i] =='&' ||*argv[i] =='|'){
               count=i; 
               countf=i; 
          }
     }

     

     // for(int j= countf+1; argv[j]!= NULL; j++){
     //     
     //      argv2[num]= argv[j]; 
     //     // printf("The sting inside the for loop for getting argv2 is : %s", argv2[num]); 
     //      num++; 
     // }

//printf("the array argv2 is: %s", argv); // why is the value for argv not being displayed properly?

     if (*argv[count]== '>'){
         //argv[count]= NULL; 
         printf("the string after the spl char is: %s\n ", argv[count+1]); 
         fd=open(argv[count+1], O_WRONLY| O_TRUNC | O_CREAT, 0600); 
         argv[count]= NULL; 
         fd_temp= dup(1);
         dup2(fd, 1); 

         pid=fork(); 
         if(pid<0){
          printf("Can't fork this function\n"); 
         }
         else if(pid>0){
          waitpid(pid, & status, 0); 
         }
         else {
          execvp(argv[0], argv);
         }

         close(fd);
         dup2(fd_temp,1); 
         close(fd_temp);
     }
     else if(*argv[count]=='<'){
          printf("the string after the spl char is: %s\n ", argv[count+1]); 
         fd=open(argv[count+1], O_RDONLY, 0600); 
         if(fd==-1){printf("The given input file doesn't exist\n");}

         else{
         argv[count]= NULL; 
         fd_temp= dup(0);
         dup2(fd, 0); 
         pid=fork(); 
         if(pid<0){
          printf("Can't fork this function\n"); 
         }
         else if(pid>0){
          waitpid(pid, & status, 0); 
         }
         else {
          execvp(argv[0], argv);
         }

         close(fd);
         dup2(fd_temp,0); 
         close(fd_temp);
         }

     }
      else if(*argv[count]=='&'){
           argv[count]= NULL;

          pid=fork();
          if(pid<0){
               printf("Unable to fork"); 
          } 
          else if(pid != 0){
               
               //printf("executing: ");
               //waitpid(pid, &status, 0); 
          }
          else{
               //printf("executing: ");
               execvp(argv[0], argv); 
          }

          
     }
      else if(*argv[count]=='|'){
          //char argn[64]= argv+ count+ 1; 
          printf("In pipe func\n"); 
          int fd[2]; 
          pipe(fd); 
          argv[count]= NULL; 
          //argn = argv+count+1; 

          pid1= fork();
               if(pid1<0){
                    printf("Can't fork\n");
               }
               else if(pid1>0){
                    pid2= fork(); 
                    if(pid2<0){
                         printf("Can't fork\n");
                    }
                    else if(pid2>0){
                         waitpid(pid1, &status, 0); 
                         //what the parent does
                    }
                    else{
                         dup2(fd[0],0); 
                         execvp(argv[count+1], argv+count+1); 
                    }

                   // waitpid(pid, &status, 0); 
               }
               else {
                    dup2(fd[1], 1); 
                    execvp(argv[0], argv); 
               }

          close(fd[0]); 
          close(fd[1]); 
         // execvp(argv[count+1], argv+count+1 );
          
     }
     else {
          

         pid=fork();
          if(pid<0){
               printf("Unable to fork"); 
          } 
          else if(pid != 0){
              waitpid(pid, &status, 0); 
          }
          else{
               execvp(argv[0], argv); 
          }

     }

}

void  main(void)
{
     char  line[1024];             /* the input line                 */
     char  *argv[64];              /* the command line argument      */

     while (1) {                   /* repeat until done ....         */
          fputs("Shell -> ",stdout);     /*   display a prompt             */
          fgets(line, 1024, stdin);              /*   read in the command line     */
          fputs("\n", stdout);
          parse(line, argv);       /*   parse the line               */
          if (strcmp(argv[0], "exit") == 0)  /* is it an "exit"?     */
               exit(0);            /*   exit if it is                */
          execute(argv);           /* otherwise, execute the command */
     }
}
