#include "utils.h"
#include <iostream>

// Check error status and print error message
void check_error(const char *msg, EUD_ERR_t errcode) {
    char error_string[150];
    uint32_t string_size;
    eud_get_error_string(errcode, error_string, &string_size);
    std::cerr << "[errcode]" << msg << "\tstatus: " << error_string << std::endl;
}
