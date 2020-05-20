#include <stdio.h>

#include "main_functions.h"
#include "settings.h"


int main(int num_args, char **args) {

    Input_data *input_data = parseInput(num_args, args);
    if (input_data->errors) {
        errorsHandler(input_data);
    }
    switch (input_data->command) {
        case 1:
            archive(input_data);
            break;
        case 2:
            unzip(input_data);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
    }


    return 0;
}
