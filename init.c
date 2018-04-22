#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

extern char ** environ;

int main() {
    /* 输入的命令行 */
    char cmd[256];
    /* 命令行拆解成的各部分，以空指针结尾 */
    char *args[128];
    while (1) {
        /* 提示符 */
        printf("# ");
        fflush(stdin);  //清空输入缓冲区
        fgets(cmd, 256, stdin);   //读入单行指令
        /* 清理结尾的换行符 */
        int i;
        for (i = 0; cmd[i] != '\n'; i++)
            ;
        cmd[i] = '\0';
        /* 拆解命令行 */
        args[0] = cmd;
        for (i = 0; *args[i]; i++)
            for (args[i+1] = args[i] + 1; *args[i+1]; args[i+1]++)
                if (*args[i+1] == ' ') {
                    *args[i+1] = '\0';
                    args[i+1]++;
                    break;
                }
        args[i] = NULL;

        /* 没有输入命令 */
        if (!args[0])
            continue;

        /* 内建命令 */
        if (strcmp(args[0], "cd") == 0) {
            char wd[4096];
	    if (chdir(args[1])>=0){
	    printf("Current dir is:\n");
            puts(getcwd(wd, 4096));
 	    }
 	    else
            {
            printf("Error path!\n");
            }
            continue;
        }
        if (strcmp(args[0], "pwd") == 0) {
            char wd[4096];
	    printf("Current dir is:\n");
            puts(getcwd(wd, 4096));
            continue;
        }
        if (strcmp(args[0], "export") == 0){
	    char *PATH_NAME;
 	    PATH_NAME=(char*)malloc(sizeof(char)*strlen(args[1]));
	    strcpy(PATH_NAME,args[1]);
	    if(putenv(PATH_NAME)!=0){
	    printf("Error!\n");
	    }
            continue;
	}
        if (strcmp(args[0], "echo") == 0){
	    char *p = getenv(args[1]);
	    if(p)
		printf("%s\n",p);
	    else
		printf("Error Path\n");
            continue;
	}
        if (strcmp(args[0], "unset") == 0){
	    unsetenv(args[1]);
            continue;
	}
        if (strcmp(args[0], "exit") == 0){
	    exit(0);
            return 1;
	}
        /* 外部命令 */
        pid_t pid = fork();
        if (pid == 0) {
            /* 子进程 */
            execvp(args[0], args);
            /* execvp失败 */
            return 255;
        }
        /* 父进程 */
        wait(NULL);
    }
}
