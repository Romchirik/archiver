//
// Created by romchirik on 20.05.2020.
//


#ifndef ARCHIVER_MAIN_FUNCTIONS_H
#define ARCHIVER_MAIN_FUNCTIONS_H

typedef struct Input_s {
    char errors;
    char command;
    unsigned char num_inputs;
    struct File_info **source;
    struct File_info *destination;
} Input_data;

struct File_info {
    char *filepath;
    FILE *file;
};

Input_data *parseInput(int num_args, char **args);

void errorsHandler(Input_data *input_data);

Input_data *archive(Input_data *data);

void unzip(Input_data *data);

#endif //ARCHIVER_MAIN_FUNCTIONS_H
