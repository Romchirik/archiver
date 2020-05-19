#include <stdio.h>
#include "archive_func.h"
#include "input_parser.h"
#include "settings.h"
#include <stdlib.h>

int bitsetGet(bitword *arr, int idx);
void bitsetSet(bitword *arr, int idx, int newval);

int main(int num_args, char **args) {

    Input_data *input_data = parseInput(num_args, args);
    if(input_data->errors){
        errorsHandler(input_data);
    }
    switch (input_data->command) {
        case 1:
            archive(input_data);
            break;
    }


    return 0;
}
