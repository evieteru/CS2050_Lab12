//Heaps

#include <stdio.h>
#include <stdlib.h>

 typedef struct node{
    int key;
    int subtreeSize;
    struct node * left;
    struct node * right;
    struct node * up;
 }Node;

 typedef struct tree{
    int errorcode;
    int height;
    struct node * head;
 }Heap;


 Node * createNode(int newkey);
 Heap * createMinHeapTree();

 void findTreeHeight(Heap * tree);
 void resize(Node * startNode, int increment);

 Heap * insertNode(Node * newnode, Heap * tree);
 Heap * heapify(Heap * tree, Node * nodeToBeSorted);

 int deleteMin(Node * top, Heap * tree);
 int * heapSort(Heap * tree);

 void freeNodes(Node * nextNode);
 void freeHeap(Heap * tree);

 void printHeap(Heap *tree);
 void printHeap2(Node * node);







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







Heap * createMinHeapTree(){

    Heap * newTree = malloc(sizeof(Heap));

    if (newTree == NULL){
        printf("Malloc in createMinHeapTree failed\n");
        return newTree;
    }

    newTree->head = NULL;
    newTree->errorcode = 0;
    newTree->height = 0;

    return newTree;
}




//This is built to be used for increasing and decreasing but is only in use for increase
void resize(Node * startNode, int increment){ //increment is for if you need to increase or decrease

    if (startNode->up == NULL){ // base case: reached the top
        return;
    }

    startNode->subtreeSize += increment; //update size

    resize(startNode->up, increment); //recursively resize the node above
}





//for use in heapify. Finds the longest path of the tree
void findTreeHeight(Heap * tree){

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





//inserts a node as a leaf at the end of the shortest path
Heap * insertNode(Node * newnode, Heap * tree) {
    
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




Heap * heapify(Heap * tree, Node * nodeToBeSorted){

    findTreeHeight(tree);
    Node * heightControl = nodeToBeSorted;

    if (heightControl == NULL){
        return tree;
    }


    for (int count = 1; count <=tree->height; count ++) {
        for (int count2 = 1; count2 <= tree->height -count; count2++){
            if(heightControl->key < heightControl->up->key){
                //swap the keys
                int tempKey = heightControl->key;
                heightControl->key = heightControl->up->key;
                heightControl->up->key = tempKey;
            }
        }
        heightControl = heightControl->up;

    }
    return (tree);
}





//frees a leaf node, recursively switches the keys to restore the invariant
int deleteMin(Node * top, Heap * tree){

    printf("in delete min\n");

    int min;

    if(top == NULL){ //checks for empty tree
        return -1;
    }

    min = top->key;

    if(top ->left == NULL && top->right == NULL){ //base case if node is a leaf
        //resize(tree->head, -1); how do I resize when the top is deleted? 

        //case for if this is a single node tree
        if(top->up == NULL){
            tree->head = NULL;
            free(top);
            return min;
        }

        // all other cases
        if(top->up->left == top){
            top->up->left = NULL;
        }
        if(top->up->right == top){
            top->up->right = NULL;
        }
        
        free(top);

        return min;
    }

    if (top->left == NULL){ //if there is no left
        top->key = deleteMin(top->right, tree); //go to the right
    }
    else{
        if(top->right == NULL){ //if there is no right
            top->key = deleteMin(top->left, tree); //go the the left
        }

        else{
            if (top->left->key < top->right->key) { //if the left is less than the right
                top->key = deleteMin(top->left, tree); //go to the left
            }
            else {
                top->key = deleteMin(top->right, tree); //if none of the above, go right
            }
        }
    }

    return min;

}






int * heapSort(Heap * tree){
    int * sortedArray;
    sortedArray = malloc(20 * sizeof(int));

    int new_entry = -1;
    int count = 0;

    if(deleteMin(tree->head, tree) == -1){ //if the tree is empty
        printf("Heap is empty\n");
        sortedArray[0] = -1;
        return sortedArray; //return an empty array
    }

    do{ //gets the min, puts it into the array, ascending order

        new_entry = deleteMin(tree->head, tree);

        sortedArray[count] = new_entry;
        count++;

    }while(new_entry != -1);

    return sortedArray;
}










void freeNodes(Node * nextNode){
     if (nextNode == NULL){ // base case
        return;
    }

    // recursively free all the Nodes
    freeNodes(nextNode->left);
    freeNodes(nextNode->right);

    nextNode->left = NULL;
    nextNode->right = NULL;
    nextNode->up = NULL;

    free(nextNode);
}



void freeHeap(Heap * tree){

    freeNodes(tree->head);
    free(tree);
}


//leaks memory, but it prints!
void printHeap(Heap *tree) {
    if (tree == NULL || tree->head == NULL) {
        printf("Heap is empty, nothing to print.\n");
        return;
    }

    Node **queue = malloc((tree->height + 1) * sizeof(Node *));
    int front = 0, rear = 0;
    int levelNodes = 1;  //Number of nodes at the current level

    queue[rear++] = tree->head; //enqueue the head

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

    //free(queue); //!!Why does this line get stuck after for loop pass 4
}


//prints a single column
void printHeap2(Node * node){
    if (node != NULL){
        printf("%d\n", node->key);

        printHeap2(node->left);
        printHeap2(node->right);
    }
}









int main() {
    // Create a min heap
    Heap * myHeap = createMinHeapTree();

    //Insert nodes into the heap
    int keys[] = {4, 2, 7, 1, 5, 6, 8};

    for (int i = 0; i < sizeof(keys) / sizeof(keys[0]); i++) {
        printf("For loop pass %d\n", i);
        Node *newNode = createNode(keys[i]);
        insertNode(newNode, myHeap);
        heapify(myHeap, newNode);
        //choose your print method
        printHeap2(myHeap->head);
        //printHeap(myHeap);
    }

    // Heap sort full
    int *sortedArray = heapSort(myHeap);

     // Display the sorted array
    printf("\nSorted Array:\n");
    if(sortedArray[0] != 0){
    for (int i = 0; sortedArray[i] != -1; i++) {
        printf("%d ", sortedArray[i]);
    }
    printf("\n");
    }
    else{
        printf("Heap was empty, nothing to print. Sincerely, main\n");
    }




    // Empty the heap
    for(int count = 0; count < 7; count++){
        int minElement = deleteMin(myHeap->head, myHeap);
        printf("\nDeleted Min Element: %d\n", minElement);
        //printHeap2(myHeap->head);
        printHeap(myHeap);
    }


    
    // Heap sort empty
    int *sortedArray2 = heapSort(myHeap);

    // Display the sorted array
    printf("\nSorted Array:\n");
    if(sortedArray2[0] != -1){
    for (int i = 0; sortedArray2[i] != -1; i++) {
        printf("%d ", sortedArray2[i]);
    }
    printf("\n");
    }
    else{
        printf("Heap was empty, nothing to print. Sincerely, main\n");
    }


    // Free the memory
    freeHeap(myHeap);
    free(sortedArray);


}

/*
 - getTreeheight and heapify only works if the tree is always balanced or not balanced by one. otherwise it'll seg fault THIS IS OK FOR LAB
 - for lab: dont worry about resize with delete, you will be given an empty tree

 - pick up where you left off: 


 Things I learned:
  - you weren't reassigning the pointers in deleteMin, you weren't reassigning the head to NULL when the tree was empty
      - BE WARY OF EDGE CASES!

*/



// //NOT IN USE! kind of works, but needs work. We don't need to switch nodes though, just keys.
// Heap * heapify2(Heap * tree, Node * nodeToBeSorted){

//     findTreeHeight(tree);
//     Node * heightControl = nodeToBeSorted;

//     if (heightControl == NULL){
//         return tree;
//     }
    
//                 while (heightControl->up != NULL && heightControl->key < heightControl->up->key){ // while not at the head and height control is still less than the node above it
//                 Node * temp = heightControl->up;

//                 if (heightControl->up->left == heightControl) {
//                     heightControl->up->left = heightControl->left;
//                     heightControl->up->right = heightControl->right;
//                     heightControl->up->up = heightControl->up;

//                     heightControl->left = temp->left;
//                     heightControl->right = temp->right;
//                     heightControl->up = temp->up;
//                 } else {
//                     heightControl->up->right = heightControl->right;
//                     heightControl->up->left = heightControl->left;
//                     heightControl->up->up = heightControl->up;

//                     heightControl->right = temp->right;
//                     heightControl->left = temp->left;
//                     heightControl->up = temp->up;
//                 }

//                 heightControl = heightControl->up;
                
//                 }
      


//     return tree;

// }