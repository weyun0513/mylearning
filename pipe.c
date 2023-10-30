/*communicate between parennt and cjild using a pipe
Parent will create the pipe
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{

    int pipeDescriprorp[2];
    int status, childProcessId, childstatus;
    int valuesToWritw[] = {1,
                           3,
                           5,
                           7,
                           9};
    int valueRead;
    int index;
    int numRead, numWrittrn, size;

    /*Parent create a pipe before creating child process*/
    status = pipe(pipeDescriprorp);
    if (status == -1)
    {
        printf("Fail to create pipe\n");
        exit(EXIT_FAILURE);
    }
    /*create child*/
    childProcessId = fork();
    switch ((childProcessId))
    {
    case -1:
        printf("Fail to create child\n");
        exit(EXIT_FAILURE);

    case 0: // Child process
        /*close the write description*/
        status = close(pipeDescriprorp[1]);
        if (status == -1)
        {
            printf("Child Fail to close write description\n");
            _exit(EXIT_FAILURE);
        }
        numRead = read(pipeDescriprorp[0], &valueRead, sizeof(int));
        while (numRead > 0)
        {
            printf("Child read: %d\n", valueRead);
            numRead = read(pipeDescriprorp[0], &valueRead, sizeof(int));
        }
        if (numRead == -1)
        {
            printf("Child fail read from pipe.");
            _exit(EXIT_FAILURE);
        }
        status = close(pipeDescriprorp[0]); // close read
        if (status == -1)
        {
            printf("Child Fail to close read description\n");
            _exit(EXIT_FAILURE);
        }
         _exit(EXIT_SUCCESS);
    default:
        status = close(pipeDescriprorp[0]);//close read
         if (status == -1)
        {
            printf("Parent Fail to close write description\n");
             exit(EXIT_FAILURE);
        }
        size=5*sizeof(int);
        numWrittrn=write(pipeDescriprorp[1],valuesToWritw,size);
        printf("numWrittrn size: %d\n",numWrittrn);
        if(numWrittrn<size){
             printf("Parent Fail to write description\n");
             exit(EXIT_FAILURE);
        }
         status = close(pipeDescriprorp[1]); // close write
         if (status == -1)
        {
            printf("Child Fail to close write description\n");
            exit(EXIT_FAILURE);
        }
          status = wait(&childstatus); // 讀取fork status 出來
       if (status == -1)
        {
            printf("Fail to wait for child to cpmplete\n");
            exit(EXIT_FAILURE);
        }
      exit(EXIT_SUCCESS);
}
}
