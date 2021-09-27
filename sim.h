#include "rngs.h"

typedef struct node {
char* type;
double clock;
struct node* next;
}node;
node* createNode(char*, double);
node* insertNode(node*, node*);
node* removeNode(node*, node**);
void printLists(node*);
//double arrival(double);
//double departure(double);

node* createNode(char* type, double clock)
{
	node* nodePtr = (node*)malloc(sizeof(node));
	nodePtr->type=type;
	nodePtr->clock=clock;
	nodePtr->next=NULL;
	
	return nodePtr;
}

node* insertNode(node* head, node* newNodePtr)
{
	if(head == NULL)
	{
		return newNodePtr;
	}
	else if (head->clock > newNodePtr->clock )
	{
		newNodePtr->next = head;
		return newNodePtr;
	}
	else
	{
		node* prev = NULL;
		node* curr = head;
		
		while((curr!=NULL)&&(curr->clock <= newNodePtr->clock))
		{
			prev = curr;
			curr = curr->next;
		}
		prev->next = newNodePtr;
		newNodePtr->next = curr;

		return head;
	}
}

node* removeNode(node* head, node** nextRef)
{
	*nextRef = head;
	
	if(head!=NULL)
	{
		head = head->next;
	}
	
    (*nextRef)->next = NULL;
    
	return head;
}

void printLists(node* head)
{
    node* printNode = head;

 
    while (printNode != NULL)
    {
        printf("type: %s clock:%2f -->", printNode->type, printNode->clock );
        printNode = printNode->next;
    }
    printf("\n");

}

/*
double arrival(double lambda){
	//printf("test...");
	//printf("%f", expTest(1, lambda));
	return expTest(1, lambda);
}

double departure(double mu){

	return expTest(2, mu);

}
*/