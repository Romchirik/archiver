#include <inttypes.h>
#include "input_parser.h"

#ifndef ARCHIVER_ARCHIVE_FUNC_H
#define ARCHIVER_ARCHIVE_FUNC_H

typedef unsigned char bitword;

typedef struct Huffman_node_s {
    uint8_t byte;
    unsigned int priority;
    struct Huffman_node_s *left;
    struct Huffman_node_s *right;
} Huffman_node;

typedef struct Byte_code_s {
    uint64_t code;
    uint64_t code_length;
} Byte_code;

typedef struct Byte_info_s {
    uint8_t byte;
    uint64_t priority;
} Byte_info;


Input_data *archive(Input_data *data);

void codesTreeBypass(Huffman_node *root, Byte_code **byte_codes_table, uint64_t code, uint64_t length);

unsigned int saveTreeBypass(Huffman_node *root, unsigned int curr_pos, int *curr_byte_pos, bitword *tree_template, unsigned char *data_template);

int getNumMoves(Huffman_node *root, int current_pos);

Huffman_node *buildHuffmanTree(Byte_info *freq_table);

#endif //ARCHIVER_ARCHIVE_FUNC_H
