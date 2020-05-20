#include <stdlib.h>
#include <stdio.h>

#include "settings.h"
#include "archive_func.h"
#include "main_functions.h"

typedef unsigned char bitword;

int bitsetGet(bitword *arr, int idx);

void unzip(Input_data *data) {

    int tree_template_len;
    short int alphabet_len;
    unsigned char num_files;

    unsigned char *alphabet = NULL;
    bitword *tree_template = NULL;

    fread(&tree_template_len, sizeof(int), 1, data->destination->file);
    tree_template = calloc(tree_template_len, sizeof(bitword));
    fread(tree_template, sizeof(bitword), tree_template_len, data->destination->file);
    tree_template_len = tree_template_len * BITSET_BLOCK_SIZE;

    fread(&alphabet_len, sizeof(short int), 1, data->destination->file);
    alphabet = calloc(alphabet_len, sizeof(unsigned char));
    fread(alphabet, sizeof(unsigned char), alphabet_len, data->destination->file);
    fread(&num_files, sizeof(unsigned char), 1, data->destination->file);

    char current_command = 0, current_byte = 0;
    Huffman_node *root = getTreeFromFile1(&current_command, &current_byte, tree_template, alphabet_len, alphabet);

    for (int i = 0; i < num_files; i++) {
        unsigned int filename_len;
        char *filename = NULL;
        unsigned long long int num_bytes;

        fread(&filename_len, sizeof(unsigned int), 1, data->destination->file);
        filename = malloc(sizeof(char) * filename_len);
        fread(filename, sizeof(char), filename_len, data->destination->file);
        fread(&num_bytes, sizeof(unsigned long long int), 1, data->destination->file);

        for(int k = 0; k < )

    }

}