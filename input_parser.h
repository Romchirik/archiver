#ifndef ARCHIVER_INPUT_PARSER_H
#define ARCHIVER_INPUT_PARSER_H

struct File_info {
    char *filepath;
    FILE *file;
};

//ERRORS
//0 - successful
//1 - wrong command
//2 - wrong archive name
//3 - archive doesn't exist
//4 - file to archive doesn't exist
//5 - few input arguments
//6 - archive already exists
//7 - no files to archive
//8 - one byte file, unable to build compress table

//COMMANDS
//1 - archive
//2 - extract
//3 - delete file from archive
//4 - info (list of files)
//5 - check archive integrity

typedef struct Input_s {
    char errors;
    char command;
    unsigned char num_inputs;
    struct File_info **source;
    struct File_info *destination;
} Input_data;

Input_data *parseInput(int num_args, char **args);
void errorsHandler(Input_data *input_data);

#endif //ARCHIVER_INPUT_PARSER_H
