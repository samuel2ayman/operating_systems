#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<signal.h>
#include<sys/resource.h>
char input[100];
char *arguments[50];
char path_arr[100];
int last;
//------------------------------------------
void Setup_Environment()
{
    chdir(getcwd(path_arr,100));
}
//-----------------------------------------
void read_input()
{
char path_arr[100];
printf("%s:~$",getcwd(path_arr,100));
fgets(input,100,stdin);
input[strlen(input)-1]='\0';
}
//-------------------------------------------
void execute_exit()
{
    exit(0);
}
//--------------------------------------------
 void parse_input()
 {int i=0;
    arguments[0]=strtok(input," ");
    if (!strcmp(arguments[0],"export"))
    {
        
       while(arguments[i]!=NULL)
    {
        arguments[++i]=strtok(NULL,"=");
        
    }
     arguments[i]='\0';
    }
    else{
    
    
    while(arguments[i]!=NULL)
    {
        arguments[++i]=strtok(NULL," ");
        
    }
    }
    last=i-1;
 }
void execute_echo()
{
    for(int i = 1; i <= last; i++)
    {
        char* str = arguments[i];
        if(str[0] == '"')
            str++;
        if(str[strlen(str)-1] == '"')
            str[strlen(str)- 1] = '\0';
        if(str[0] == '$')
        {
            str++;
            printf("%s",getenv(str));
        }
        else
            printf("%s",str);
        printf(" ");
    }
    printf("\n");
}
 void execute_pwd()
 {
  printf("%s\n",getcwd(path_arr,100));
 }
 //----------------------------------------
 void execute_cd()
 {
   if((arguments[1] == NULL) || (!(strcmp(arguments[0],"cd~")))){
        chdir("/home/samuel_ayman"); 
    }
    else{
        
         chdir(arguments[1]); 
         
    }
 }
 void execute_export(){
    char *variable= arguments[2];
    if(variable[0] == '"'){
        variable++;
        variable[strlen(variable)-1] = '\0';
        setenv(arguments[1] , variable , 1);
    }
    else{
     
        setenv(arguments[1] , arguments[2] , 1);
    }
    if (variable != NULL) {
        printf("%s=%s\n",arguments[1],variable);
    } else {
        printf("environment variable is not set\n");
    }
 }
 
//------------------------------------
void execute_shell_bultin()

    {
        if(!strcmp(arguments[0],"echo"))
        {
            execute_echo();
        }
        else if(!strcmp(arguments[0],"pwd"))
        {
            execute_pwd();
        }
        else if(!strcmp(arguments[0],"cd"))
        {
            execute_cd();
        }
        else if(!strcmp(arguments[0],"export"))
        {
        execute_export();
        }
    }
void execute_command (void)
{
    int Error = 0;
    int child_id= fork();
    if(child_id < 0)
    {
        printf(" Error fork failed!\n");
       execute_exit();
    }
    else if (child_id ==0)
    {
        for(int i = 0, j = 0; i <= last; i++)
        {
            char* env_var = arguments[i];
            if(env_var[0] == '$')
            {
                char* ptr_env;
                env_var++;
                ptr_env = getenv(env_var);
                char* argument = strtok(ptr_env, " ");
                while(argument != NULL)
                {
                    arguments[j++] = argument;    
                    argument = strtok(NULL, " ");
                }
            }
            else
                j++;
        }
        Error = execvp(arguments[0], arguments);
        if(Error)
            printf("Error! unknown command\n");
    }
    else
    {
        if(!strcmp(arguments[last], "&"))
            return;
        else
        {
            waitpid(child_id, NULL, 0);
            return;
        }
    }
}

void shell() 
{
      while(1)
    {
       read_input();
       parse_input();
       if(!strcmp(arguments[0],"exit"))
       {
         execute_exit();
       }
       else if((!strcmp(arguments[0],"echo"))||(!strcmp(arguments[0],"pwd"))||(!strcmp(arguments[0],"cd"))||(!strcmp(arguments[0],"export")))
       {
        execute_shell_bultin();
       }
       else
       {
        execute_command();
       }

    }
 }

void Reap_Child_Zombie() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        printf("Child process %d terminated\n", pid);
    }
    
    if (pid == -1) {
        return;
    }
}

void Write_To_Log_File(void){
    FILE * file = fopen("log.text" , "appended");
    if(file == NULL){
        printf("Error in file\n");
       execute_exit();
    }
    else{
        fprintf(file , "%s" , "Child process terminated\n");
        fclose(file);
    }
}
void on_child_exit(){
    Reap_Child_Zombie();
    Write_To_Log_File();
}

void Register_Child_Signal()
{
    signal(SIGCHLD , on_child_exit);
}
int main(){
   Register_Child_Signal();
   Setup_Environment();
   shell();
   return 0;
}