#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    pid_t pid;
    pid_t myrealid;
    pid_t myparentid;

    printf ("\nBEFORE FORK\n");

    pid = fork();

    //printf ("\nAFTER FORK\n");

    switch (pid)
    {
        case 0:
        myrealid = getpid();
        myparentid = getppid();
            printf("\n\nPROCESS API - CHILD (myPid= %d) (ParentPid = %d)\n\n", myrealid, myparentid);
            break;

        case -1:
            printf("\n\nPROCESS API FAIL - PARENT\n\n");
            break;

        default:
            myrealid = getpid();
            printf("\n\nPROCESS API - PARENT (myPid= %d) (childPid = %d)\n\n", myrealid, pid);
            break;
    }


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