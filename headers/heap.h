#ifndef HUFFMAN_HEAP_H
#define HUFFMAN_HEAP_H

#include <malloc.h>
#include <stdio.h>
#include "header.h"

struct m_node {
    int val;
    char key;
    struct m_node* left;
    struct m_node* right;
};

typedef struct m_node node_;

struct m_heap{
    node_* arr[MAX_SIZE];
    int size;
};

typedef struct m_heap heap_;

node_* new_node(char key, int val);
void print_node(node_* node, int dp, char* str);
node_* get_tree(int freq[MAX_SIZE]);
void free_node(node_* node);

void heapify_top(heap_* heap, int i);
void insert(heap_* heap, node_* val);
node_* pop(heap_* heap);

#endif // HUFFMAN_HEAP_H