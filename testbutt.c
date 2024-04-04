//Anthony Snyder
//4/3/2024
//Assignment #4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19

typedef struct customer { //structure for a customer
    char name[MAXLEN + 1];
    int points;
} customer;

typedef struct treenode { //structure for a tree node
    customer* cPtr; //pointer to a customer
    int size; //number of nodes in the subtree rooted at this node
    struct treenode* left;
    struct treenode* right;
} treenode;

customer* createCustomer(const char* name, int points) { //create a new customer
    customer* newCustomer = (customer*)malloc(sizeof(customer));
    strcpy(newCustomer->name, name);
    newCustomer->points = points;
    return newCustomer;
}

treenode* createNode(customer* c) { //create a new tree node
    treenode* newNode = (treenode*)malloc(sizeof(treenode));
    newNode->cPtr = c;
    newNode->size = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

treenode* insert(treenode* root, customer* c) { //insert a customer into the binary search tree
    if (root == NULL) {
        return createNode(c);
    }

    if (strcmp(c->name, root->cPtr->name) < 0) { //insert into the left subtree
        root->left = insert(root->left, c);
    }
    else if (strcmp(c->name, root->cPtr->name) > 0) { //insert into the right subtree
        root->right = insert(root->right, c);
    }
    else { //if customer already exists, update points
        root->cPtr->points += c->points;
    }

    root->size++; //increment size of subtree rooted at this node
    return root;
}

treenode* search(treenode* root, const char* name, int* depth) { //search for a customer in the binary search tree
    *depth = 0;
    while (root != NULL && strcmp(root->cPtr->name, name) != 0) {
        (*depth)++;
        if (strcmp(name, root->cPtr->name) < 0) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return root;
}

treenode* deleteNode(treenode* root, const char* name) { //delete a node from the binary search tree
    if (root == NULL) {
        return root;
    }

    if (strcmp(name, root->cPtr->name) < 0) { //search for the node to be deleted
        root->left = deleteNode(root->left, name);
    } else if (strcmp(name, root->cPtr->name) > 0) {
        root->right = deleteNode(root->right, name);
    } else {
        if (root->left == NULL) { //node found, delete it
            treenode* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            treenode* temp = root->left;
            free(root);
            return temp;
        }

        treenode* temp = root->left; //if node has two children, replace with successor
        while (temp->right != NULL) {
            temp = temp->right;
        }
        root->cPtr = temp->cPtr;
        root->left = deleteNode(root->left, temp->cPtr->name);
    }
    root->size--; //decrement size of subtree rooted at this node
    return root;
}

int countSmaller(treenode* root, const char* name) { //count number of customers with names before a given name alphabetically
    int count = 0;
    while (root != NULL) {
        if (strcmp(root->cPtr->name, name) < 0) {
            count += root->left ? root->left->size + 1 : 1; //include left subtree size
            root = root->right;
        } else {
            root = root->left;
        }
    }
    return count;
}

void inorderTraversal(treenode* root, customer** sortedCustomers, int* index) { //perform in-order traversal and store pointers to customers in sorted order
    if (root == NULL) {
        return;
    }
    inorderTraversal(root->left, sortedCustomers, index);
    sortedCustomers[(*index)++] = root->cPtr;
    inorderTraversal(root->right, sortedCustomers, index);
}

void merge(customer** arr, int l, int m, int r) { //perform merge step of merge sort
    int n1 = m - l + 1;
    int n2 = r - m;

    customer* L[n1];
    customer* R[n2];

    for (int i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->points > R[j]->points || (L[i]->points == R[j]->points && strcmp(L[i]->name, R[j]->name) <= 0)) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void mergeSort(customer** arr, int l, int r) { //perform merge sort
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

int main() { //main function
    int n;
    scanf("%d", &n); //read the number of commands

    treenode* root = NULL;
    customer* customers[n]; //array to store pointers to customers

    for (int i = 0; i < n; i++) { //loop through each command
        char cmd[20], name[MAXLEN + 1];
        int points, depth;
        scanf("%s", cmd); //read the command

        if (strcmp(cmd, "add") == 0) { //process each command type
            scanf("%s %d", name, &points);
            customer* c = createCustomer(name, points);
            root = insert(root, c);
            printf("%s %d\n", name, c->points);
            customers[i] = c;
        } else if (strcmp(cmd, "sub") == 0) {
            scanf("%s %d", name, &points);
            treenode* node = search(root, name, &depth);
            if (node == NULL) {
                printf("%s not found\n", name);
            } else {
                node->cPtr->points = (node->cPtr->points >= points) ? node->cPtr->points - points : 0;
                printf("%s %d\n", name, node->cPtr->points);
            }
        } else if (strcmp(cmd, "del") == 0) {
            scanf("%s", name);
            treenode* node = search(root, name, &depth);
            if (node == NULL) {
                printf("%s not found\n", name);
            } else {
                printf("%s deleted\n", name);
                root = deleteNode(root, name);
            }
        } else if (strcmp(cmd, "search") == 0) {
            scanf("%s", name);
            treenode* node = search(root, name, &depth);
            if (node == NULL) {
                printf("%s not found\n", name);
            } else {
                printf("%s %d %d\n", name, node->cPtr->points, depth);
            }
        } else if (strcmp(cmd, "count_smaller") == 0) {
            scanf("%s", name);
            int count = countSmaller(root, name);
            printf("%d\n", count);
        }
    }

    int index = 0;
    customer* sortedCustomers[n];
    inorderTraversal(root, sortedCustomers, &index);
    mergeSort(sortedCustomers, 0, n - 1);

    for (int i = 0; i < n; i++) { //output sorted customers
        printf("%s %d\n", sortedCustomers[i]->name, sortedCustomers[i]->points);
    }

    for (int i = 0; i < n; i++) { //free memory
        free(customers[i]);
    }

    return 0;
}
