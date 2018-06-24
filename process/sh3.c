#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MAX_CMD 200
#define MAX_PATH 200

/*split comannd strings*/
int split(char *buf,char**argv){

	while(*buf == ' '){
		buf++;
	}

	int delim = 0;
	int argc = 0;

	while(*buf != '\n'){
		while(buf[delim] != '\n' && buf[delim] != ' '){
			delim++;
		}

		if(buf[delim] == '\n'){
			buf[delim] = '\0';
			argv[argc++] = buf;
			break;
		}
		buf[delim] = '\0';
		argv[argc++] = buf; 
		buf += delim+1;
		delim = 0;
		
		while(*buf==' '){
			buf++;
		}
	}
	argv[argc] = NULL;		/*the last element is NULL*/
	return 0;
}

/*handle built-in command*/
int built_in_cmd(char **argv){

	if(strcmp(argv[0], "cd")==0){		/*change cur_path to the des_path*/
		if(chdir(argv[1])){				
			printf("[myshell]$ cd:%s: No such directory!\n",argv[1]);
		}
		return 1;
	}

	if(strcmp(argv[0], "pwd")==0){
		char buf[MAX_PATH];
		printf("%s\n",getcwd(buf, sizeof(buf)));
		return 1;
	}

	if(strcmp(argv[0], "exit")==0){
		exit(0);
	}

	return 0;	
}

/*analyse command*/
void eval(char *cmd_str){	

	char *argv[MAX_CMD];		/*split the command strings to argv*/
	char buf[MAX_CMD];			/*store modified command*/
	int count;					/*store the numbers of command chars*/
	strcpy(buf, cmd_str);

	split(buf, argv);			/*split command*/
	
	if(argv[0] == NULL)			/*empty command*/
		return;					
	if(built_in_cmd(argv)) 		/*builtin command*/
		return;
	if(strcmp(argv[0], "echo") == 0){/*analyse the redirect command 'echo'*/
		int i;
		int flag = 0;
		for(i = 1; argv[i] != NULL; i++){	/*command include '>' char*/
			if(*argv[i] == '>'){
				flag = 1;
				break;
			}
		}	
		if(flag == 1){						/*represent redirect command*/
			FILE *fp = fopen(argv[i + 1], "w+");
			int j = 0;							
			for(j = 1; j < i; j++){			/*write to the file*/
				fseek(fp, 0, SEEK_END);
				fwrite(argv[j], strlen(argv[j]), 1, fp);
				fseek(fp, 0, SEEK_END);
				fprintf(fp,"\n");
			}
			fclose(fp);
			return;
		}
	}
	if(strcmp(argv[0], "cat") == 0){
		int i;
		int flag = 0;
		for(i = 1; argv[i] != NULL; i++){
			if((*argv[i] == 'w') && (*(argv[i]+1) == 'c')){			//analyse the pipe command 'cat'
				flag = 1;  
				break;
			}
		}
		if(flag == 1){
			int pid;
			int fd[2];
			char buf[16];

			pipe(fd);
			pid = fork();
			if(pid == 0){
		 	 	dup2(fd[1], 1);
				close(fd[0]);
				close(fd[1]);
				execlp("cat", "cat", argv[1], NULL);
				exit(0); 
			}
			
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			read(0, buf, sizeof(buf));
			execlp("wc", "wc", "-l", NULL);
			return;
		}
	}

	int pid;
	pid = fork();
	if(pid == 0){
		if(execvp(argv[0], argv) < 0){
			printf("Non-existent command:%s.\n", argv[0]);
			exit(0);
		}
	}
	wait(NULL);
}

int main(int argc, char *argv[]){

	char cmd_str[MAX_CMD];
	int n;
	while(1){
		printf("[myshell]$ ");
		fflush(stdout);						
		
		if((n = read(0, cmd_str, MAX_CMD)) < 0){	/*read command*/
			printf("Falied to read command!");
		}								
		eval(cmd_str);
	}
	return 0;
}
