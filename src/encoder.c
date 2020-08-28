#include "../headers/encoder.h"

void encode() {
    char* data[MAX_SIZE];
    printf("Start encoding\n");
    int file_len = encode_fill_freq_table(data);
    encode_write_bin(data);
    encode_write_table(data, file_len);
    printf("Finished encoding\n");
}

// fills frequency table and counts characters
int encode_fill_freq_table(char* data[]) {
    int freq[MAX_SIZE] = { 0 };
    FILE* fin = fopen(SOURCE_TEXT_NAME, "r");
    if (fin == NULL) { printf("No source text file"); exit(EXIT_FAILURE); }

    char c;
    int file_len = 0;
    while(fscanf(fin, "%c", &c) > 0) {
        freq[c]++;
        file_len++;
    }
    fclose(fin);

    char str[MAX_SIZE] = { 0 };
    node_* tree = get_tree(freq);
    encode_tree_to_table(data, tree, 0, str);
    return file_len;
}

// completes table from tree
void encode_tree_to_table(char* data[], node_* tree, int depth, char* str) {
    if (tree == NULL) return;
    if (depth == 0) for(int i = 0; i < MAX_SIZE; i++) data[i] = NULL;

    if (tree->key != 0) {
        data[tree->key] = malloc(sizeof(char) * (depth + 1));
        if(data[tree->key] == NULL) { printf("Memory allocation error"); exit(EXIT_FAILURE); }
        memcpy(data[tree->key], str, sizeof(char) * (depth + 1));
    }

    str[depth] = '0';
    encode_tree_to_table(data, tree->left, depth + 1, str);
    str[depth] = '1';
    encode_tree_to_table(data, tree->right, depth + 1, str);
    str[depth] = 0;

    free(tree);
}

void encode_write_bin(char* data[]) {
    FILE* fin = fopen(SOURCE_TEXT_NAME, "r");
    if (fin == NULL) { printf("No source text file"); exit(EXIT_FAILURE); }
    FILE* out = fopen(BIN_FILE_NAME, "wb");
    if (out == NULL) { printf("Can't open bin file"); exit(EXIT_FAILURE); }

    char c, cbuf = 0;
    int i = 0, j;
    // completes data into bytes and prints them to file
    while(fscanf(fin, "%c", &c) > 0) {
        j = 0;
        while(i < 8 && data[c][j] != '\0') {
            cbuf <<= 1;
            cbuf += data[c][j] == '1';
            i++; j++;
            if (i == 8) {
                fwrite(&cbuf, 1, 1, out);
                i = 0;
            }
        }
    }
    // is the last byte wasn't completed, fill it with zeroes
    if(i != 0) {
        while (i < 8) {
            cbuf <<= 1;
            i++;
        }
        fwrite(&cbuf, 1, 1, out);
    }
    fclose(fin);
    fclose(out);
}

void encode_write_table(char* data[], int file_len) {
    FILE* fout = fopen(TABLE_FILE_NAME, "w");
    fprintf(fout, "%d\n", file_len);
    for(int i = 0; i < MAX_SIZE; i++) {
        if (data[i] != NULL) {
            fprintf(fout, "%c %s\n", i, data[i]);
        }
    }
    fclose(fout);
}



