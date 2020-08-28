#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include <stdio.h>
#include <stdbool.h>

struct m_tree{
    struct m_tree* left;
    struct m_tree* right;
    char val;
};

struct m_bit_reader{
    FILE* f;
    int i;
    bool cur;
    char buf;
};

typedef struct m_tree tree_;
typedef struct m_bit_reader bit_reader_;

tree_* new_tree();
void decode_create_tree(FILE* f, tree_* t, char val, int dp);
bool read_bit(bit_reader_* r, int* pok);
char decode_char(tree_* root, bit_reader_* read, int* pok);
void decode_text(tree_* root, bit_reader_* read, int file_len);
void decode();

#endif // HUFFMAN_DECODER_H
