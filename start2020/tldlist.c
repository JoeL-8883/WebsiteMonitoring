#include "date.h"
#include "tldlist.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "date.h"

typedef struct tldlist TLDList;
typedef struct tldnode TLDNode;
typedef struct tlditerator TLDIterator;

struct tldlist {
    struct date *begin;
    struct date *end;
    TLDNode *root;
    long addCount; // Number of times a date node has been successfully added
    int uniqueNodes; // Number of unique tlds
};

struct tldnode {
    char *key;
    int count;
    TLDNode *left;
    TLDNode *right;
};

struct tlditerator {
    int index;
    int size;
    TLDNode *iterator[10000000]; // Array of 1 million nodes
};

TLDList *tldlist_create(Date *begin, Date *end) {
    struct tldlist *l;
    
        if ((l = (struct tldlist *)malloc(sizeof(struct tldlist))) != NULL) {
            l->begin = begin;
            l->end = end;
            l->addCount = 0;
            l->uniqueNodes = 1; // initially 1 for the very first node in the tree
        }

    return l;
}

void destroyBST_nodes(TLDNode *root) {
    if(root == NULL) {
        return;
    }
    // BST traversal to go through each node and free
    destroyBST_nodes(root->left);
    destroyBST_nodes(root->right);
    free(root);
    root = NULL; // set the pointer to null
}

void tldlist_destroy(TLDList *tld) {
    destroyBST_nodes(tld->root);
    free(tld);
}

// Add a node into the correct position in the tree
void add_tree(TLDNode *node, TLDNode *root, TLDList *l) {
    if(strcmp(node->key, root->key) < 0) {
        if(root->left) {
            add_tree(node, root->left, l);
        } else {
            root->left = node;
            l->uniqueNodes++;
        }
    
    } else if(strcmp(node->key, root->key) > 0) {
        if(root->right) {
            add_tree(node, root->right, l);    
        } else {
            root->right = node;
            l->uniqueNodes++;
        }
    // The strings are equal
    } else {
        root->count++;
    }
}

TLDNode *create_first_node(char *hostname) {
    struct tldnode *node;
    if ((node = (struct tldnode *)malloc(sizeof(struct tldnode))) != NULL) {
        node->key = hostname;
        node->count = 1;
    }

    return node;
}
 
int tldlist_add(TLDList *tld, char *hostname, Date *d) {
    // Take the tld substring
    char *seperator = strrchr(hostname, '.') + 1;
    int len_seperator = strlen(seperator);
    char *tld_code = (char *)malloc(sizeof(char) * (len_seperator + 1));
    tld_code[len_seperator] = '\0'; 
    strcpy(tld_code, seperator);
    
    // Check if the date is within the list boundary
    if(date_compare(d, tld->begin) >= 0 && date_compare(d, tld->end) <= 0) {
        // Check if the tree is empty
        if(tld->root == NULL) {
            // Create the first node
            tld->root = create_first_node(tld_code);
        }    else {
            // Create a new node or add to the count
            struct tldnode *node;
            if ((node = (struct tldnode *)malloc(sizeof(struct tldnode))) != NULL) {
                node->key = tld_code;
                node->count = 1;
                add_tree(node, tld->root, tld);
            }
        }

    tld->addCount++;
    return 1;     

    // Added unsuccessfully
    } else {
        return 0;
    }
            
    
}

long tldlist_count(TLDList *tld) {
    return tld->addCount;
}

// Convert the BST to an array by BST traversal
void flatten(TLDNode *root, TLDIterator *i) {
    if(root == NULL) { 
        return;
    } else {
        flatten(root->left, i);
        i->iterator[i->index] = root;
        i->index++;
        flatten(root->right, i);
    }
}

/*
void tldlist_iter_destroyNodes(TLDIterator *i) {
    int length = i->size;
    for(int i=0; i < length; i++) {
        free(i->iterator[i]);
    }
}
*/

TLDIterator *tldlist_iter_create(TLDList *tld) {
    // Creating pointer to root element of iterator
    struct tlditerator *i;
    if ((i = (struct tlditerator *)malloc(sizeof(struct tldnode)*tld->uniqueNodes)) != NULL) {
        i->index = 0;
        i->size = tld->uniqueNodes;
        flatten(tld->root, i);
        i->index = 0;
    }

    return i;
}

// Return the next element in the array then increment or return NULL when reached the end of the array
TLDNode *tldlist_iter_next(TLDIterator *iter) {
    if(iter->index == iter->size) {
        return NULL;
    } else {
        return iter->iterator[iter->index++];
    }
}



void tldlist_iter_destroy(TLDIterator *iter) {
    free(iter);
}

char *tldnode_tldname(TLDNode *node) {
    return node->key;
}

long tldnode_count(TLDNode *node) {
    return node->count;
}