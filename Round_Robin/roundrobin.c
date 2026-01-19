#include <stdlib.h>
#include <stdio.h>

#define UNITTIME 2;
int next_pid = 1;

typedef struct _Process
{
    int pid;
    char name;
    int size;

    struct _Process * next;
    
} Process;

struct _Queue
{
    Process* first;
    int count;

};

typedef struct _Queue Queue;

Queue* create_queue();
Process* create_process(char, int);
int enqueue(Queue* q, Process* p); //we use int instead bc linuz will return negative when fails
Process* dequeue(Queue* p);

int rr(Queue*);

int main()
{
    Queue *rrq = create_queue();
    Process* pA = create_process('A', 4);
    Process* pB = create_process('B', 6);
    Process* pC = create_process('C', 1);

    enqueue(rrq, pA);
    enqueue(rrq, pB);
    enqueue(rrq, pC);


    printf("\n\n\nSTARTING THE SCHEDULER\n\n\n");
    rr(rrq);
    printf("\n\n\nSTOPPING THE SCHEDULER\n\n\n");


    free(rrq);
    return 0;
}



Queue* create_queue()
{
    Queue* result = malloc(sizeof(Queue));
    if(result == NULL)
    {
        return NULL;   
    }

    result->count = 0; //equivalent is (*result).count = 0;
    result->first = NULL;

    return result;

    //this is a one liner to do everything: return calloc(sizeof(Queue));
}

Process* create_process(char name, int size)
{
    Process* result;

    result = malloc(sizeof(Process));

    if(result == NULL)
    {
        return NULL;
    }

    result->pid = next_pid++;
    result->name = name;
    result->size = size;
    result->next = NULL;


    return result;
}



int enqueue(Queue* q, Process* p)
{
    if (q == NULL) return -1;
    if (p == NULL) return 0;

    if (q->first == NULL)
    {
        q->first = p;
        q->count = 1;
        return 0;
    }

    Process* current = q->first;

    while (current->next != NULL)
    {
        current = current->next;
    }


    current->next = p;
    ++q->count;
    return 0;

}


Process* dequeue(Queue* q)
{
    if (q == NULL) return -1;
    if (q->first == NULL) return NULL;

    --q->count;
    Process* result = q->first;
    q->first = q->first->next;

    result->next = NULL;
    return result;
}

int rr (Queue* q)
{
    if (q == NULL) return -1;

    Process *p;

    while (q->count > 0) //alternative: while (q.first != NULL)
    {
        p = dequeue(q);
        ////
        //printf("Process p.name is running");
        p->size -= UNITTIME;
        printf(" %c ", p->name);
        ////
        
        if (p->size > 0)
        {
            enqueue(q, p);
        }
        else
        {
            free(p);
        }
        
    }


    return 0;
}