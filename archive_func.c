#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <unistd.h>

#include "settings.h"
#include "archive_func.h"
#include "main_functions.h"

typedef unsigned char bitword;

bitword *createBitset(int length) {
    return calloc(length, sizeof(unsigned char));
}

Byte_info *createFreqTable(unsigned int length) {
    uint8_t counter = 0;
    Byte_info *tmp = calloc(length, sizeof(Byte_info));
    for (unsigned int i = 0; i < length; i++) {
        tmp[i].byte = counter;
        counter++;
    }
    return tmp;
}

int compareNode(const void *a, const void *b) {

    Byte_info tmp_a = *(Byte_info *) a;
    Byte_info tmp_b = *(Byte_info *) b;

    return (tmp_a.priority > tmp_b.priority) - (tmp_a.priority < tmp_b.priority);
}

short int fillFreqTable(Byte_info *freq_table, struct File_info **source, int num_files) {
    short int counter = 0;
    uint8_t byte_buffer[READING_BUFFER_LEN];

    for (int i = 0; i < num_files; i++) {
        FILE *tmp_file = source[i]->file;

        while (!feof(tmp_file)) {
            unsigned int total_bytes = READING_BUFFER_LEN;
            unsigned int tmp = fread(byte_buffer, 1, READING_BUFFER_LEN, tmp_file);
            if (tmp != READING_BUFFER_LEN) {
                total_bytes = tmp;
            }

            for (unsigned int j = 0; j < total_bytes; j++) {
                freq_table[byte_buffer[j]].priority++;
            }

        }
    }

    qsort(freq_table, FREQ_TABLE_LEN, sizeof(Byte_info), compareNode);
    for (int i = 0; i < FREQ_TABLE_LEN; i++) {
        if (freq_table[i].priority != 0) counter++;
    }
    return counter;
}

void codeFile(struct File_info *file_to_code, struct File_info *destination) {
    unsigned char write_buffer[WRITE_BUFFER_LEN];

    unsigned char filename_len = strlen(file_to_code->filepath);
    char reading_available = 1;

    struct stat filestat;
    int descriptor = open(file_to_code->filepath, O_RDONLY);
    fstat(descriptor, &filestat);
    close(descriptor);

    fwrite(&filename_len, sizeof(unsigned int), 1, destination->file);
    fwrite(&file_to_code->filepath, sizeof(char), filename_len, destination->file);

    fclose(file_to_code->file);
    while (reading_available) {
        int i;

        if (i != READING_BUFFER_LEN - 1) {
            reading_available = 0;
            fclose(file_to_code->file);
        }


    }
}

Input_data *archive(Input_data *data) {

    Huffman_node *root = NULL;
    Byte_info *freq_table = createFreqTable(FREQ_TABLE_LEN);
    short int non_zero_bytes = fillFreqTable(freq_table, data->source, data->num_inputs);


    root = buildHuffmanTree(freq_table);

    Byte_code **byte_codes_table = calloc(FREQ_TABLE_LEN, sizeof(Byte_code *));
    codesTreeBypass(root, byte_codes_table, 0, 0);
    free(freq_table);

    //saving Huffman tree

    int tree_template_len = getNumMoves(root, 0);
    tree_template_len = tree_template_len / BITSET_BLOCK_SIZE;
    bitword *tree_template = calloc(tree_template_len, sizeof(bitword));
    unsigned char *alphabet = calloc(non_zero_bytes, sizeof(unsigned char));

    tree_template_len = (tree_template_len / BITSET_BLOCK_SIZE) + 1;

    int tmp = 0;
    saveTreeBypass(root, 0, &tmp, tree_template, alphabet);

    //optimizing tree
    for (int i = tree_template_len - 1; i >= 0; i++) {
        if (tree_template[i] == 0) {
            tree_template_len--;
        } else {
            break;
        }
    }

    //write archive header
    fwrite(&tree_template_len, sizeof(int), 1, data->destination->file);
    fwrite(tree_template, sizeof(bitword), tree_template_len, data->destination->file);

    fwrite(&non_zero_bytes, sizeof(short int), 1, data->destination->file);
    fwrite(alphabet, sizeof(unsigned char), non_zero_bytes, data->destination->file);
    fflush(data->destination->file);
//
//    for (int i = 0; i < data->num_inputs; i++) {
//        codeFile(data->source[i], data->destination);
//    }




}
