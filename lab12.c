#include <stdlib.h>
#include <stdio.h>
#include <limits.h>


typedef struct _Heap Heap;

typedef struct _Heap{
    int height;
    struct node * head;
}Heap;



typedef struct node{
    int key;
    int subtreeSize;
    struct node * left;
    struct node * right;
    struct node * up;
 }Node;


typedef Heap * PHeap;

struct _Employee
{
	char fname[256];
	char mname[256];
	char lname[256];
	char tshirtsize;
};
typedef struct _Employee Employee;

// Prototypes
PHeap HeapConstruct();
int HeapInsert(PHeap heap, int key, void * inputData);
int HeapDelete(PHeap heap, void ** outputData);
void HeapDestruct(PHeap heap);
void PrintHeap(PHeap heap);


Node * createNode(int newkey);
PHeap insertNode(Node * newnode, PHeap tree);
PHeap heapify(PHeap tree, Node * nodeToBeSorted);
void findTreeHeight(PHeap tree);
void resize(Node * startNode, int increment);







PHeap HeapConstruct()
{
	PHeap newTree = malloc(sizeof(Heap));

    if (newTree == NULL){
        printf("Malloc in HeapConstruct failed \n");
        return newTree;
    }
    
    newTree->head = NULL;
    newTree->height = 0;

    return newTree;
}


int HeapInsert(PHeap heap, int key, void * inputData)
{
    if(heap == NULL){
        return -1;
    }


    Node * newnode = createNode(key);

    if (newnode == NULL){ //if malloc failed in create node
        return -1;
    }

    insertNode(newnode, heap);


    heapify(heap, newnode);

    return 0;
	
}


int HeapDelete(PHeap heap, void ** outputData)
{
	return -1;
}


void HeapDestruct(PHeap heap)
{
}




//END GRADED FUNCTIONS

//////////////////////////////////////////////////////////////////////////////////
//creates a node
Node * createNode(int newkey){

    Node * newNodePtr = malloc(sizeof(Node));

    if (newNodePtr == NULL){
        return newNodePtr;
    }

    newNodePtr->key = newkey;
    newNodePtr->left = NULL;
    newNodePtr->right = NULL;
    newNodePtr->up = NULL;
    newNodePtr->subtreeSize = 0;

    return newNodePtr;
}






PHeap insertNode(Node * newnode, PHeap tree){
//First node inserted
    if (tree->head == NULL){
        tree->head = newnode;
        return tree;
    }


    //All other cases
    Node * traverseptr = tree->head;

    while(1){ //finds shortest path and inserts a node as a leaf

        if(traverseptr->subtreeSize == 0){ //if the node is a leaf
            traverseptr->left = newnode; //add the node to the left
            newnode->up = traverseptr; //newnode->up points to the node above it in the tree
            traverseptr->subtreeSize += 1; //add one to the size 
            break;
        }

        else{
            if (traverseptr->left == NULL){ //if the left is empty
                traverseptr->left = newnode; //insert node to the left
                newnode->up = traverseptr; //assign the up member
                traverseptr->subtreeSize += 1; //increase the size of the node above it
                break;
            }
            else if (traverseptr->right == NULL){ //same but if the right is empty
                traverseptr->right = newnode;
                newnode->up = traverseptr;
                traverseptr->subtreeSize += 1;
                break;
            }
            else if (traverseptr->left->subtreeSize <= traverseptr->right->subtreeSize){ //if the left tree is smaller
                traverseptr = traverseptr->left; //move traverse to the left
            }
            else{ // if the right tree is smaller
                traverseptr = traverseptr->right; //move traverse to the right
            }
        }
    }
    return (tree);

}










PHeap heapify(PHeap tree, Node * nodeToBeSorted){

    findTreeHeight(tree);
    Node * heightControl = nodeToBeSorted;

    if (heightControl == NULL){
        return tree;
    }

    while(heightControl != NULL && heightControl->up != NULL){
    //for (int count = 1; count <=tree->height; count ++) {
        //for (int count2 = 1; count2 <= tree->height -count; count2++){
            if(heightControl->key < heightControl->up->key){
                //swap the keys
                int tempKey = heightControl->key;
                heightControl->key = heightControl->up->key;
                heightControl->up->key = tempKey;
            
        //}
        heightControl = heightControl->up;
            }
            else{break;}

    //}
    }
    return (tree);
}









//for use in heapify. Finds the longest path of the tree
void findTreeHeight(PHeap tree){

    if (tree == NULL){
        return;
    }

    tree->height = 0; //reset height
    Node * traverseptr = tree->head;

    

    while(1){ //finds longest path and counts the nodes
    //printf("Height is %d\n", tree->height);

        if(traverseptr->left == NULL && traverseptr->right == NULL){ //if it is a leaf node
            tree->height += 1;
            break;
        }
        else{
        
        tree->height += 1;

        if(traverseptr->left == NULL){ // if the left is empty
            traverseptr = traverseptr->right; //go right
        }
        else if (traverseptr->right == NULL){ //if right is empty
            traverseptr = traverseptr->left; //go left
        }        
        else if(traverseptr->left->subtreeSize >= traverseptr->right->subtreeSize){ //if the left side is bigger
            traverseptr = traverseptr->left; //go to the left
        }
        else{ //if right is bigger
            traverseptr = traverseptr->right; //go to the right 
        }
        }
        

    }

    return;
}









//This is built to be used for increasing and decreasing but is only in use for increase
void resize(Node * startNode, int increment){ //increment is for if you need to increase or decrease

    if (startNode->up == NULL){ // base case: reached the top
        return;
    }

    startNode->subtreeSize += increment; //update size

    resize(startNode->up, increment); //recursively resize the node above
}










void PrintHeap(PHeap heap){
    if (heap == NULL || heap->head == NULL) {
        printf("Heap is empty, nothing to print.\n");
        return;
    }

    Node **queue = malloc((heap->height + 1) * sizeof(Node *));
    int front = 0, rear = 0;
    int levelNodes = 1;  //Number of nodes at the current level

    queue[rear++] = heap->head; //enqueue the head

    while (rear > front) { // while the queue isnt empty
        Node *current = queue[front++]; //dequeues first node in line

        printf("%d ", current->key); // prints its key

        // checks if the current node has children
        if (current->left != NULL) { //if there is a left child
            queue[rear++] = current->left; //enqueue it
        }
        if (current->right != NULL) {
            queue[rear++] = current->right;
        }

        levelNodes -= 1; //decrement levelNodes
        if (levelNodes == 0) { //if there are no more nodes on this level
            printf("\n"); //print a new line
            levelNodes = rear - front;
        }
    }
}























#include <stdlib.h>
#include <stdio.h>


void PrintEmployee(Employee * p)
{
	if (p)
	{
		printf("\t%s %s %s (%c)\n",p->fname, p->mname, p->lname, p->tshirtsize);
	}
	else
	{
		printf("\tNULL pointer!\n");
	}
}

int main() 
{
	int array[] = { 8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 15 };

	PHeap heap = HeapConstruct();

	for (int i=0;i<sizeof(array)/sizeof(array[0]); i++)
	{
		printf("Inserting %d\n",array[i]);
		HeapInsert(heap, array[i],NULL);
		PrintHeap(heap);
		printf("\n\n");
	}

	//printf("\nHeap:\n");
	//PrintHeap(heap);
	//printf("\n\n");

	// for (int i=0;i<sizeof(array)/sizeof(array[0]); i++)
	// {
	// 	void * temp;
	// 	int key=HeapDelete(heap,&temp);
	// 	printf("%d deleted\n",key);
	// }

	// HeapDestruct(heap);

	// // Make sure we don't leak memory
	// heap=HeapConstruct();
	// HeapDestruct(heap);

	// // Try adding something more interesting to the heap
	// Employee employees[] =
	// {
	// 	{"George","W","Bush",'z'},
	// 	{"James","Edward","Ries",'M'},
	// 	{"Laura","Christine","Ries",'m'},
	// 	{"Murphy","","Ries",'X'},
	// 	{"Cisco","","Ries",'S'},
	// 	{"James","AlternateDimension","Ries",'L'},
	// 	{"James","EvilJimR","Ries",'X'},
	// 	{"Lawrence","David","Ries",'x'},
	// 	{"Paul","David","Polly",'L'},
	// 	{"Alan","Jerome","Lammers",'X'},
	// 	{"Earl","Daniel","Kraus",'l'},
	// 	{"William","David","Moser",'X'},
	// 	{"William","Jefferson","Clinton",'A'},
	// };
	// int count = sizeof(employees)/sizeof(employees[0]);
	// heap=HeapConstruct();

	// for (int i=0;i<count;i++)
	// {
	// 	HeapInsert(heap, count-i,&(employees[i]));
	// }

	// printf("\nEmployees should be in reverse order as we delete them:\n");
	// for (int i=0;i<count; i++)
	// {
	// 	Employee * pEmp;
	// 	HeapDelete(heap,(void *)&pEmp);
	// 	PrintEmployee(pEmp);
	// }

	// HeapDestruct(heap);

	// Try a few NULL checks
	HeapInsert(NULL,0,NULL);
	HeapDelete(NULL,NULL);
	HeapDestruct(NULL);

}
