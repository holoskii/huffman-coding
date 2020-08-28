#include <malloc.h>
#include "../headers/decoder.h"
#include "../headers/header.h"

tree_* new_tree() {
    tree_* tree = (tree_*)malloc(sizeof(tree_));
    tree->left = tree->right = NULL;
    tree->val = 0;
    return tree;
}

void decode_create_tree(FILE* f, tree_* t, char val, int dp) {
    char c = fgetc(f);
    if (c == EOF) {
        return;
    }
    if (c == '\n') {
        t->val = val;
        return;
    }
    if (c == '0') {
        if (t->left == NULL) t->left = new_tree();
        decode_create_tree(f, t->left, val, dp + 1);
    }
    if (c == '1') {
        if (t->right == NULL) t->right = new_tree();
        decode_create_tree(f, t->right, val, dp + 1);
    }
}

bool read_bit(bit_reader_* r, int* pok) {
    if (r->i >= 8) {
        if (fread(&(r->buf), 1, 1, r->f) == 0) {
            *pok = 0;
            return 0;
        }
        r->i = 0;
    }

    bool ret = (r->buf & (1 << 7)) > 0;
    r->cur = ret;
    (r->buf) <<= 1;
    (r->i)++;
    *pok = 1;
    return ret;
}

char decode_char(tree_* root, bit_reader_* read, int* pok) {
    if (read->cur == 0) {
        if (root->left == NULL) return root->val;
        if (*pok == 0) return 0;
        read_bit(read, pok);
        return decode_char(root->left, read, pok);
    }
    else {
        if (root->right == NULL) return root->val;
        if (*pok == 0) return 0;
        read_bit(read, pok);
        return decode_char(root->right, read, pok);
    }
}

void decode_text(tree_* root, bit_reader_* read, int file_len) {
    int i = 0;
    int ok = 1;
    FILE* fout = fopen(OUTPUT_TEXT_NAME, "w");
    while(i < file_len && ok) {
        ok = 1;
        char c = decode_char(root, read, &ok);
        //printf(".%d", c);
        fprintf(fout, "%c", c);
        i++;
    }
    fclose(fout);
}

void decode() {
    printf("Start decoding\n");
    FILE* f = fopen(TABLE_FILE_NAME, "r");
    int file_len;
    fscanf(f, "%d", &file_len); fgetc(f);
    tree_* root = new_tree();
    while(1) {
        char c;
        c = fgetc(f);
        fgetc(f);
        //printf("'%c'\n", c);
        decode_create_tree(f, root, c, 0);
        if (c == EOF) break;
    }
    fclose(f);

    bit_reader_* r = (bit_reader_*)malloc(sizeof(bit_reader_));
    r->f = fopen(BIN_FILE_NAME, "rb");
    if (fread(&(r->buf), 1, 1, r->f) == 0) {
        return;
    }
    r->cur = (r->buf & (1 << 7)) > 0;
    (r->buf) <<= 1;
    r->i = 1;

    decode_text(root, r, file_len);
    printf("Finished decoding\n");
}
