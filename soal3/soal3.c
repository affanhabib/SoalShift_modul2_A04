#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
    pid_t child1, child2, child3;
    int pipes[2], status1, status2, status3;
    char isi[100000];
    
    pipe(pipes);
    child1 = fork();
    if(child1 == 0)
    {
        char *unzip[3] = {"unzip", "campur2.zip", NULL};
        execv("/usr/bin/unzip", unzip);
    }
    else
    {    
        while ((wait(&status1)) > 0);
        child2 = fork();
        if(child2 == 0)
        {
            dup2(pipes[1], 1);
            close(pipes[0]);
            close(pipes[1]);
            
            char *ls[3] = {"ls", "campur2", NULL};
            execv("/bin/ls", ls);
        }
        else
        {
            while ((wait(&status2)) > 0);

            child3 = fork();
            if(child2 == 0)
            {
                dup2(pipes[0], 0);
                close(pipes[1]);
                close(pipes[0]);

                char *grep[3] = {"grep", ".txt$", NULL};
                execv("/bin/grep", grep);

                int store = read(pipes[1],isi,sizeof(isi));
                FILE *daftar = fopen("daftar.txt", "w+");
                fputs(isi, daftar);
                fclose(daftar);
            }
        }
    }
}
