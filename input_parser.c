#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "settings.h"

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

char getCommand(char *command) {
    unsigned int length = strlen(command);
    if (length != 2) {
        return -1;
    } else {
        if (command[0] == '-') {
            switch (command[1]) {
                case 'a':
                    return 1;
                case 'x':
                    return 2;
                case 'd':
                    return 3;
                case 'i':
                    return 4;
                case 't':
                    return 5;
            }
        } else {
            return -1;
        }
    }
    return -1;
}

Input_data *parseInput(int num_args, char **args) {
    Input_data *input_data = calloc(1, sizeof(Input_data *));

    //check num args
    if (num_args < 3) {
        input_data->errors = 5;
        return input_data;
    }

    //trying to get archiver command
    char tmp_command = getCommand(args[1]);
    if (tmp_command == -1) {
        input_data->errors = 1;
        return input_data;
    } else {
        input_data->command = tmp_command;
    }

    //checking input archive name and existing
    if (strstr(args[2], ".ext") == NULL) {
        input_data->errors = 2;
        return input_data;
    }
    if (access(args[2], F_OK) != -1) {
        if (tmp_command == 1) {
            input_data->errors = 6;
            return input_data;
        }
    } else {
        if (tmp_command == 2 || tmp_command == 3 || tmp_command == 4 || tmp_command == 5) {
            input_data->errors = 3;
            return input_data;
        }
    }
    if(tmp_command == 1){
        input_data->destination = calloc(1, sizeof(struct File_info));
        input_data->destination->filepath = args[2];
        input_data->destination->file = fopen(args[2], "wb");

    } else {
        input_data->destination = calloc(1, sizeof(struct File_info));
        input_data->destination->filepath = args[2];
        input_data->destination->file = fopen(args[2], "rb");
    }

    input_data->num_inputs = num_args - 3;


    switch (tmp_command) {
        case 1:
            if (input_data->num_inputs > 0) {
                input_data->source = calloc(input_data->num_inputs, sizeof(Input_data *));
            } else {
                input_data->errors = 7;
                return input_data;
            }
            for (int i = 3; i < num_args; i++) {
                input_data->source[i - 3] = calloc(1, sizeof(struct File_info));
                input_data->source[i - 3]->filepath = args[i];
                input_data->source[i - 3]->file = fopen(args[i], "rb");

                if (input_data->source[i - 3]->file == NULL) {
                    input_data->errors = 4;
                }
            }
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }

    return input_data;
}

void errorsHandler(Input_data *input_data) {
    switch (input_data->errors) {
        case 1:
            printf("Wrong archiver command, available commands:\n");
            printf("'-a' - archive files\n");
            printf("'-x' - extract files\n");
            printf("'-i' - show files in the archive\n");
            printf("'-d' - delete file from the archive\n");
            printf("'-t' - check archive integrity\n");
            break;
        case 2:
            printf("Wrong archive name\n");
            break;
        case 3:
            printf("Archive doesn't exist\n");
            break;
        case 4:
            //Надо когда нить доделать эту хуйню
            printf("File(s)  ");

            break;
        case 5:
            printf("Few input arguments, use the format below:\n");
            printf("%s", INPUT_STRING_FORMAT"\n");
            break;
        case 6:
            printf("Archive already exists, use other name\n");
            break;
        case 7:
            printf("");
            break;
        default:
            printf("WTF you printed here if you reached this error???\n");
    }
}