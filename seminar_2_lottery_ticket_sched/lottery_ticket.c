#include <stdio.h>//standard input output
#include <stdlib.h>//standard library lets us use malloc, free, atoi, random
#include <string.h>//lets us use string commands like strtok
#include <time.h>// lets us use time command

#define UNITTIME 1;//how many tickets are subtracted each win. 

typedef struct _Process//typedef means we need to define an alias at the end. struct means were creating a structure (a user defined data-type that lets us group multiple variable types)
{

    char name;
    int size;

    struct _Process * next;
    
} Process;

typedef struct _Queue
{
    Process* first;
    int count;

} Queue;

Queue* create_queue();
Process* create_process(char, int);
int enqueue(Queue* q, Process* p);//we use int instead bc linux will return negative when fails
Process* dequeue(Queue* p); 
int wwcd(Queue*, int totaltkts);//winner winner chicken dinner


int main()
{
    srand(time(NULL));//rand function isn't actually random, so we have to seed it with something using srand. srand does not return any values, all it does is let us set a seed so that we get something more random. time() returns the value of time in seconds since the epoch (epoch is january 1, 1970 UTC) is returned.

    printf("Welcome to the Lottery Ticket Scheduler!\n\nBegin by entering the number of processes you would like to create.\n");

    char input1[64];//how many chars can fit in input
    int ticketsEach[64];//stores the process's ticket amounts
    char* token;//used to keep passing pointers of the seperate ticket amounts
    int num_Proc = 0;//used to store how many proccesses the user wishes to use
    int token_check = 0;//i used it to get out of a big loop with another condition to ensure it was exiting correctly.
    int i = 0;//was used a couple of times for incrementing
    int totalTickets = 0;//used to store the total amount of tickets


    while (num_Proc == 0)//number of processes cannot be zero ._. who will win the lottery?
    {
        fgets(input1, sizeof(input1), stdin);//used to read input. (where data will be stored, maximum size of string (size of returns size of variable in bytes), what stream to read data from(stdin is default input stream))
        num_Proc = atoi(input1);//converts the initial portion of string to an int but does not detect specific errors. returns converted value or 0 on error.

        if (num_Proc == 0)//num_Proc only == 0 when atoi has an error converting to int
            printf("\nYou entered an invalid input. Please try again\n");
    }


    printf("\nOK, you would like to create %d processes. Now enter the amount of tickets in order for each process.\n(Make sure to separate with spaces only. Ex: 10 20 30 40)\n", num_Proc);


    while (token_check == 0)//we need this loop to go on to keep tokenizing the input for ticket values.
    {
        fgets(input1, sizeof(input1), stdin);//used to read input. (where data will be stored, maximum size of string (size of returns size of variable in bytes), what stream to read data from(stdin is default input stream))
        token = strtok(input1, " \n\t");//strtok helps us seperate a string into chunks called tokens. it is seperated based on the delimiter which in this case is a space, \n, \t. it replaces this delimiter with \0 (null). it returns a pointer to the token, or NULL if there are no more.
        
        while (token != NULL)//as long as there is something to read this will happen
        {
            ticketsEach[++i] = atoi(token);// we chose to pre-increment to be able to check if any ticket count is 0. if we used post incrementing it would assign a value then increment, making us check ticketsEach[i-1] and liviu likes preincrements and predecrements hopefully it makes him happy
            if (i > num_Proc)//since we are pre incrementing I will, at most, be == num_Proc. so if its greater, the user assigned too many values
            {
                printf("\nWe detected too many assigned ticket values. This is not possible, start over.\n");
                i = 0;//reset i to 0 since user needs to start over. haha
                totalTickets = 0;//also because they have to start over. haha
                break;//since they need to start over we break out inner while loop and restart.
            }
            else if (ticketsEach[i] == 0)//a process cannot have 0 tickets, thats not fair :C, also for invalid characters
            {
                printf("\nWe detected a process having an invalid value of tickets. This is not possible, start over.\n");
                i = 0;//reset i to 0 since user needs to start over. haha
                totalTickets = 0;//also because they have to start over. haha
                break;//since they need to start over we break out inner while loop and restart.
            }
            else
            {
                if (i<=num_Proc)//check to make sure we are within the amount of processes defined.
                {
                    totalTickets += ticketsEach[i];//so we can get the total amount of tickets.
                    token = strtok(NULL, " ");//strtok remembers where it leaves off (the null character that replaces the delimiter with) which is why we use NULL instead of input. it still checks for delimiters.
                }
                if ((token == NULL) && (i != (num_Proc)))//check to make sure too little ticket values are not inputted. token will turn null and exit loop if user desnt put in tickets for each process.
                {
                    printf("\nWe detected too little assigned ticket values. This is not possible, start over.\n");
                    i = 0;//reset i to 0 since user needs to start over. haha
                    totalTickets = 0;//also because they have to start over. haha
                    break;//since they need to start over we break out inner while loop and restart.
                }
            }
        }

        if ((token == NULL) && (i == num_Proc))//to make sure that we actually are out, gets us out this bigass while loop and resets i because i use it again later
        {
            token_check = 1;//breaks us out of while loop
            i = 0;//resets i in case we need it later
        }

    }

    Queue *lottery = create_queue();//creates a queue and assigns it to lottery
    Process* processes[num_Proc+1];//did this because if size of array is size X, then indexes will be from 0 to (X-1)

    for (i = 1; i<=num_Proc; i++)//runs num_Proc times
    {
        char name = 'A' + (i - 1);//when the char 'A' is added to an int, that char uses its ASCII value.
        processes[i] = create_process(name, ticketsEach[i]);//create, assign, and enqueue processes.
        enqueue(lottery, processes[i]);
    }


    printf ("\nSTARTING THE LOTTERY\n");
    wwcd(lottery, totalTickets);//starts the lottery ticket schedule
    printf ("\nSTOPPING THE LOTTERY\n");


    free(lottery);//free the memory allocated to the queue. idk liviu likes it

}


Queue* create_queue()
{
    Queue* result = malloc(sizeof(Queue));//allocates memory and returns pointer to the allocated memory, sizeof(Queue) calculates how many bytes the Queue structure takes up
    if(result == NULL)//malloc return null when it fails
    {
        return NULL;   
    }

    result->count = 0;//equivalent is (*result).count = 0; this is a count of how many processes in the queue
    result->first = NULL;// sets first to null so that it is ready for the first process to be set

    return result;//returns pointer to the memory allocated for the queue

}

Process* create_process(char name, int size)
{
    Process* result;

    result = malloc(sizeof(Process));//allocates memory and returns pointer to the allocated memory, sizeof(Process) calculates how many bytes the Process structure takes up

    if(result == NULL)//malloc failed if it returns null
    {
        return NULL;
    }

    result->name = name;//assigns given char as process name
    result->size = size;//assigns given size as process size
    result->next = NULL;//sets next pointer to null, we need this so that ina linked list it points to the next pointer


    return result;//returns pointer value to the memory allocated for the process
}

int enqueue(Queue* q, Process* p)
{
    if (q == NULL) return -1;//if the queue is null return -1 indicating error
    if (p == NULL) return 0;//means the process was empty and we do nothing

    if (q->first == NULL)//if there are no processes, do these steps for enqueuing the first process.
    {
        q->first = p;//set first process as this process that meets the if case
        q->count = 1;//set the count to 1
        return 0;//since its the first enqueue it will end the function here
    }

    Process* current = q->first;//sets current to the process that is the first process.

    while (current->next != NULL)//this while loop is for when we have more than one process in the queue, otherwise it skips. it will stop at the end of the linked list
    {
        current = current->next;//this sets current to the current pointers next value (the next process)
    }


    current->next = p;//sets the last nodes next pointer in the linked list to the process just enqueued
    ++q->count;//increments the queues count so that it is accurate
    return 0;
}

Process* dequeue(Queue* q)//since this is for RR it only used for dequeuing the head. if a process is in a different position this is not useful
{
    if (q == NULL) return -1;//if queue return null then there was an error
    if (q->first == NULL) return NULL;//if there is no first process then the queue is empty

    Process* result = q->first;//we save our queues first process in a different variable
    q->first = q->first->next;//now we change the queues first process to the originals next value
    q->count--;// the count is decremented since we got rid of one

    result->next = NULL;//we break any ties so that the Process is no longer linked to the next process
    return result;
}

int wwcd (Queue* q, int totaltkts)
{
    if (q == NULL) return -1;//if queue returns null an error occurred so we return -1

    while ((q->count > 0) && (totaltkts>0))//while the q is not empty and their is still tickets in the lottery
    {
        int winner = (rand() % totaltkts) + 1;//winner is chosen at "random". since this is seeded with time since epoch, we get a better random than the default. since we use modulo with the total amount of tickets + 1 we make sure that the range we get is from 1-total tickets. a lotter winner cant have 0 tickets duh
        int tracker = 0;//tracker is to keep track of the added size of tickets when checking who the winner is. since it is set to zero here, it resets after every winner ensuring proper values

        Process* prev = NULL;//previous variable for adding ticket sizes to get >= winner value. also to make sure we can remove a node and re link the list properly
        Process* current = q->first;//sets the current as the queues first process

        while (current != NULL)//as long as the queue is not empty
        {
            tracker += current->size;//tracker will add the value of currents ticket count and so on, resets after it iterates again up there
            if (tracker >= winner) break;//if the current processes ticket count is greater than or equal to the winning number than it wins
            prev = current;//if current processes ticket count is less than winner we set it to previous so we can check next process
            current = current->next;//current is overwritten with the originals next pointer to the next process
        }

        --current->size;//decrement the winning processes amount of tickets 
        --totaltkts;//decrement the total amount of tickets out there so that a number that is not possible doesnt win
        printf("\n%c had %d and now has %d, total tickets is %d, and winning number is %d\n", current->name,current->size + 1,current->size, totaltkts, winner);

        if (current->size == 0)//if a process has no more tickets
        {
            if (prev == NULL)//if the processes previous is null, meaning if its the head of the linked list
            {
                dequeue(q);//it uses dequeue which will take out the head process, set its next to NULL. previous is then set to null all over again at the beginning of this main while loop. q.count is decremented in dequeue as well
                free(current);//we free the memory allocated for the process
            }
            else//if the process is not the head
            {
                prev->next = current->next;//the process before it has its next pointer set to the currents next pointer, removing current process from the linked list. (check with chat i think current is still linked to current.next which means two things are linked to current.next)
                --q->count;//q.count is decremented for accurate count. //we do this manually since we arent using dequeue
                free(current);//we free the memory allocated for the process
            }
        }
        
    }

    return 0;
}