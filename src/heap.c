#include "../headers/heap.h"

node_* new_node(char key, int val) {
    node_* node = (node_*)malloc(sizeof(node_));
    node->key = key;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void print_node(node_* node, int dp, char* str) {
    if (node == NULL) return;
    if (node->key != 0) printf("'%c' %s\n", node->key,  str);

    str[dp] = '0';
    print_node(node->left, dp + 1, str);
    str[dp] = '1';
    print_node(node->right, dp + 1, str);
    str[dp] = 0;
}

// makes a binary tree out of frequency table
node_* get_tree(int freq[MAX_SIZE]) {
    heap_* heap = (heap_*) malloc(sizeof(heap_));
    heap->size = 0;

    for(int i = 0; i < MAX_SIZE; i++) {
        if (freq[i] > 0) {
            insert(heap, new_node(i, freq[i]));
        }
    }
    while(heap->size > 1) {
        node_* left = pop(heap), * right = pop(heap);
        node_* j = new_node(0, left->val + right->val);
        j->left = left; j->right = right;
        insert(heap, j);
    }
    node_* j = pop(heap);
    free(heap);
    return j;
}

void free_node(node_* node) {
    if (node == NULL) return;
    free_node(node->left);
    free_node(node->right);
    free(node);
}

void heapify_top(heap_* heap, int i) {
    int left = i * 2 + 1, right = i * 2 + 2;
    int min = i;
    if (left  < heap->size && (heap->arr[left ])->val < (heap->arr[min])->val) min = left;
    if (right < heap->size && (heap->arr[right])->val < (heap->arr[min])->val) min = right;
    if (min != i) {
        node_* temp = heap->arr[min];
        heap->arr[min] = heap->arr[i];
        heap->arr[i] = temp;
        heapify_top(heap, min);
    }
}

void insert(heap_* heap, node_* val) {

    int i = heap->size;

    heap->arr[i] = val;
    int par_node = (i - 1) / 2;
    while(i != 0 && heap->arr[par_node]->val > heap->arr[i]->val) {
        node_* temp = heap->arr[par_node];
        heap->arr[par_node] = heap->arr[i];
        heap->arr[i] = temp;
        i = par_node;
        par_node = (i - 1) / 2;
    }
    (heap->size)++;
}

node_* pop(heap_* heap) {
    node_* val = heap->arr[0];
    heap->arr[0] = heap->arr[--(heap->size)];
    heapify_top(heap, 0);
    return val;
}