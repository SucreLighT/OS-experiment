#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define MAX_CMD 200
#define MAX_PATH 200

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
	argv[argc] = NULL;	
	return 0;
}

int built_in_cmd(char **argv){

	if(strcmp(argv[0], "cd")==0){		
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


void eval(char *cmd_str){	

	char *argv[MAX_CMD];		
	char buf[MAX_CMD];		
	int count;				
	strcpy(buf, cmd_str);

	split(buf, argv);		
	
	if(argv[0] == NULL)		
		return;					
	if(built_in_cmd(argv)) 		
		return;
		
	if(strcmp(argv[0], "echo") == 0){
		int i;
		int flag = 0;
		for(i = 1; argv[i] != NULL; i++){	
			if(*argv[i] == '>'){	// �����е�i��Ϊ'>'�ַ���˵��echo��ʾ�ض������� 
				flag = 1;
				break;
			}
		}	
		if(flag == 1){						// �ض������� 
			FILE *fp = fopen(argv[i + 1], "w+"); // �½��ļ� 
			int j = 0;							
			for(j = 1; j < i; j++){			
				fseek(fp, 0, SEEK_END);		// ��λ���ļ�β 
				fwrite(argv[j], strlen(argv[j]), 1, fp);	// ��'>'ǰ������д���ļ� 
				fseek(fp, 0, SEEK_END);
				fprintf(fp,"\n");
			}
			fclose(fp);
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
