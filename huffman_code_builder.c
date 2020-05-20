#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

#include "settings.h"

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

int bitsetGet(bitword *arr, int idx) {
    return (arr[idx / BITSET_BLOCK_SIZE] >> (BITSET_BLOCK_SIZE - 1 - (idx % BITSET_BLOCK_SIZE))) & 1;
}

void bitsetSet(bitword *arr, unsigned int idx, int newval) {
    if (newval) {
        arr[idx / BITSET_BLOCK_SIZE] =
                arr[idx / BITSET_BLOCK_SIZE] | (1 << (BITSET_BLOCK_SIZE - 1 - idx % BITSET_BLOCK_SIZE));
    } else {
        arr[idx / BITSET_BLOCK_SIZE] =
                arr[idx / BITSET_BLOCK_SIZE] & ~(1 << (BITSET_BLOCK_SIZE - 1 - idx % BITSET_BLOCK_SIZE));
    }
}

typedef struct Heap_s {
    int length;
    Huffman_node **data_arr;
} Heap;

void swap(Huffman_node **a, Huffman_node **b) {
    Huffman_node *tmp = *a;
    *a = *b;
    *b = tmp;
}

Heap *createHeap(int max_len) {
    Heap *tmp = calloc(1, sizeof(Heap *));
    tmp->data_arr = malloc(sizeof(Huffman_node *) * max_len);
    for (int i = 0; i < max_len; i++) {
        tmp->data_arr[i] = NULL;
    }
    return tmp;
}

void heapifyDown(Heap *target, int start_index) {
    while (2 * start_index + 1 < target->length) {
        int left = 2 * start_index + 1;
        int right = 2 * start_index + 2;
        int tmp = left;

        if (right < target->length && (target->data_arr[right]->priority < target->data_arr[left]->priority))
            tmp = right;
        if (target->data_arr[start_index]->priority <= target->data_arr[tmp]->priority) break;

        swap(&target->data_arr[start_index], &target->data_arr[tmp]);
        start_index = tmp;
    }
}

void heapifyUp(Heap *target, int start_index) {
    while (target->data_arr[start_index]->priority < target->data_arr[(start_index - 1) / 2]->priority) {
        swap(&target->data_arr[start_index], &target->data_arr[(start_index - 1) / 2]);
        start_index = (start_index - 1) / 2;
    }
}

void insertCellHeap(Heap *target, Huffman_node *data) {
    target->data_arr[target->length] = data;
    target->length++;
    heapifyUp(target, target->length - 1);
}

Huffman_node *getMinHeap(Heap *target) {
    Huffman_node *tmp = target->data_arr[0];
    target->data_arr[0] = target->data_arr[target->length - 1];
    target->length--;
    heapifyDown(target, 0);
    return tmp;
}

int getFreqTableLen(Byte_info *freq_table) {
    for (int i = 0; i < FREQ_TABLE_LEN; i++) {
        if (freq_table[i].priority != 0) {
            return FREQ_TABLE_LEN - i;
        }
    }
    return 0;
}

Huffman_node *buildHuffmanTree(Byte_info *freq_table) {

    int priority = 0;

    Huffman_node *root = NULL;
    Heap *heap = createHeap(getFreqTableLen(freq_table));

    for (int i = 0; i < FREQ_TABLE_LEN; i++) {
        if (freq_table[i].priority == 0)
            continue;
        else {
            priority++;
            Huffman_node *tmp_node = calloc(1, sizeof(Huffman_node));
            tmp_node->byte = freq_table[i].byte;
            tmp_node->priority = priority;
            insertCellHeap(heap, tmp_node);
        }
    }

    while (heap->length != 1) {
        Huffman_node *left = getMinHeap(heap);
        Huffman_node *right = getMinHeap(heap);

        Huffman_node *tmp_root = malloc(sizeof(Huffman_node));
        tmp_root->right = right;
        tmp_root->left = left;
        tmp_root->priority = left->priority + right->priority;

        insertCellHeap(heap, tmp_root);
    }
    root = getMinHeap(heap);
    free(heap);
    return root;
}

char isLeaf(Huffman_node *node) {
    if ((node->right == NULL) && (node->right == NULL)) {
        return 1;
    }
    return 0;
}

void codesTreeBypass(Huffman_node *root, Byte_code **byte_codes_table, uint64_t code, uint64_t length) {

    if (isLeaf(root)) {
        byte_codes_table[root->byte] = malloc(sizeof(Byte_code));
        byte_codes_table[root->byte]->code = code;
        byte_codes_table[root->byte]->code_length = length;
        return;
    }
    uint64_t left_code = (code << 1) | 0;
    uint64_t right_code = (code << 1) | 1;
    codesTreeBypass(root->left, byte_codes_table, left_code, length + 1);
    codesTreeBypass(root->right, byte_codes_table, right_code, length + 1);

}

int getNumMoves(Huffman_node *root, int current_pos) {
    if (isLeaf(root)) {
        return current_pos + 1;
    }
    current_pos = getNumMoves(root->left, current_pos + 1);
    current_pos = getNumMoves(root->right, current_pos + 1);
    return current_pos + 1;
}

unsigned int saveTreeBypass(Huffman_node *root, unsigned int curr_pos, int *curr_byte_pos, bitword *tree_template,
                            unsigned char *data_template) {
    if (isLeaf(root)) {
        bitsetSet(tree_template, curr_pos, UP);
        *curr_byte_pos = *curr_byte_pos + 1;
        data_template[*curr_byte_pos] = root->byte;
        return curr_pos + 1;
    }

    bitsetSet(tree_template, curr_pos, DOWN);
    curr_pos = saveTreeBypass(root->left, curr_pos + 1, curr_byte_pos, tree_template, data_template);
    bitsetSet(tree_template, curr_pos, DOWN);
    curr_pos = saveTreeBypass(root->right, curr_pos + 1, curr_byte_pos, tree_template, data_template);

    bitsetSet(tree_template, curr_pos, UP);

    return curr_pos + 1;
}
//
//Huffman_node *getTreeFromFile(Input_data *data) {
//    int tree_template_size;
//    short int alphabet_len;
//    unsigned char *alphabet = NULL;
//    bitword *tree_template = NULL;
//
//    fread(&tree_template_size, sizeof(int), 1, data->destination->file);
//    tree_template = calloc(tree_template_size, sizeof(bitword));
//    fread(tree_template, sizeof(bitword), tree_template_size, data->destination->file);
//
//    fread(&alphabet_len, sizeof(short int), 1, data->destination->file);
//    alphabet = calloc(alphabet_len, sizeof(unsigned char));
//    fread(alphabet, sizeof(unsigned char), alphabet_len, data->destination->file);
//
//    int inserted_bytes_counter = 0;
//    char previous_command = 1;
//    for (int i = 0; i < tree_template_size && inserted_bytes_counter < alphabet_len; i++) {
//        char command = bitsetGet(tree_template, i);
//        if (command) {
//
//        } else {
//
//        }
//    }
//
//
//}

Huffman_node *getTreeFromFile1(char *current_command, char *current_byte, bitword *tree_template,
                               short int alphabet_len, unsigned char *alphabet) {

    Huffman_node *root = calloc(1, sizeof(Huffman_node));
    if (*current_byte == alphabet_len - 1) {
        return root;
    }
    if (bitsetGet(tree_template, *current_command) == DOWN) {
        if (root->left == NULL) {
            ++*current_command;
            root->left = getTreeFromFile1(current_command, current_byte, tree_template, alphabet_len, alphabet);
        } else {
            ++*current_command;
            root->right = getTreeFromFile1(current_command, current_byte, tree_template, alphabet_len, alphabet);
        }
    } else {
        if (isLeaf(root)) {
            root->byte = alphabet[*current_byte];
            ++*current_byte;
        }
    }

    return root;
}