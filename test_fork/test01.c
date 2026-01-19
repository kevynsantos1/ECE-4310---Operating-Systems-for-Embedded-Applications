#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    pid_t pid;
    pid_t myrealid;
    pid_t myparentid;
    int newstate;


    printf("\nThe app name is: %s\n", argv[0]);
    printf ("\nBEFORE FORK\n");

    pid = fork();

    //printf ("\nAFTER FORK\n");

    switch (pid)
    {
        case 0:
        myrealid = getpid();
        myparentid = getppid();
            printf("\n\nPROCESS API - CHILD (myPid= %d) (ParentPid = %d)\n\n", myrealid, myparentid);

            execl("/usr/bin/ls", "/usr/bin/ls", (char *) NULL );

            

            break;

        case -1:
            printf("\n\nPROCESS API FAIL - PARENT\n\n");
            break;

        default:
            waitpid(pid, &newstate, 0);
            myrealid = getpid();
            printf("\n\nPROCESS API - PARENT (myPid= %d) (childPid = %d)\n\n", myrealid, pid);
            break;
    }

    //use fget for seminar

    /*if (pid == 0)
    {
        printf("\n\nPROCESS API - CHILD\n\n");
    }
    else 
    {
        printf ("\n\nPROCESS API - PARENT\n\n");
    }
    
    */

    return 0;
}