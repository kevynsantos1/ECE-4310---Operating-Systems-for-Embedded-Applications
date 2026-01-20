#include <stdio.h>

typedef struct _Node
{

    int count;
    char value;
    struct _Node * next;

} Node;


typedef struct _List
{

    Node * first;

}List;


Node * createNode(char c)
{

    Node * result = (Node*) malloc(sizeof(Node));
    result->next=NULL;
    result->value = c;
    result->count = 1;

}

List * createList()
{

    List * result = (List*) malloc(sizeof(List));
    result->first = NULL;


}

void insertList(List* ls, char c)
{
    if (ls==NULL) return;
    if((c>='a' && c<='z')||(c>='A' && c<='Z'))
    {
        if (ls->first == NULL) 
        {
            ls->first = createNode(c);
        }

        Node * prev = NULL;
        Node * curr = ls->first;

        while (curr && curr->value > c)
        {
            prev = curr;
            curr = curr->next;


        }
        if (prev==NULL)
        {
            Node* n = createNode(c);
            n->next = ls->first;
            ls->first = n;
            return;
        }
        if (prev->value == c)
        {
            ++prev->count;
            return;
        }

        Node * n = createNode(c);
        n->next = curr;
        prev->next = n;

    }

}


void display(List* ls)
{
    Node * cr = ls->first;

    while(cr)
    {
        printf("%c:%d", cr->value, cr->count);
        cr = cr->next;
    }
    printf("\n");
}



int main()
{

}