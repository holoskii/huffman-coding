#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include <mem.h>
#include "heap.h"

void encode();
int encode_fill_freq_table(char* data[]);
void encode_tree_to_table(char* data[], node_* tree, int depth, char* str);
void encode_write_bin(char* data[]);
void encode_write_table(char* data[], int file_len);

#endif // HUFFMAN_ENCODER_H
