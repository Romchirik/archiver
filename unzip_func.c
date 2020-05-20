#include <stdlib.h>
#include <stdio.h>

#include "settings.h"
#include "archive_func.h"
#include "main_functions.h"

void unzip(Input_data *data) {

    int tree_template_size;
    short int alphabet_len;
    unsigned char *alphabet = NULL;
    bitword *tree_template = NULL;

    fread(&tree_template_size, sizeof(int), 1, data->destination->file);
    tree_template = calloc(tree_template_size, sizeof(bitword));
    fread(tree_template, sizeof(bitword), tree_template_size, data->destination->file);

    fread(&alphabet_len, sizeof(short int), 1, data->destination->file);
    alphabet = calloc(alphabet_len, sizeof(unsigned char));
    fread(alphabet, sizeof(unsigned char), alphabet_len, data->destination->file);

    char current_command = 0, current_byte = 0;
    Huffman_node *root = getTreeFromFile1(&current_command, &current_byte, tree_template, alphabet_len, alphabet);
}