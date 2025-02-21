/* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause */
#include "lib/inc/eud_api.h"
#include "lib/inc/jtag_api.h"
#include "lib/inc/eud.h"
#include "lib/inc/eud_api.h"
#include "lib/inc/usb.h"
#include "lib/inc/ctl_api.h"
#include "lib/inc/com_api.h"
#include "lib/inc/swd_api.h"
#include "lib/inc/eud_error_defines.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char **argv) {
    uint32_t device_id = 0; // 设置正确的设备ID
    EUD_ERR_t errcode;
    const char *help_msg = "Invalid Parameters. Please check usage.\n"
                           "Usage:\n"
                           "\t%s <options> [device_id]\n\n"
                           "\tavailable options:\n"
                           "\t\tSCAN: scan device id\n"
                           "\t\tRST: reset device\n"
                           "\t\tRST_AST: assert reset\n"
                           "\t\tRST_DEAST: de-assert reset\n"
                           "\t\tSWD: enable SWD\n"
                           "\t\tJTAG: enable JTAG\n"
                           "\t\tCON: enable COM\n"
                           "\t\tTRACE: enable TRACE\n";

    // Parse Args
    if (argc != 3 && argc != 2) {
        // Print usage if arg numbers not meet.
        goto help_exit;
    }

    // assign device_id
    if (argc == 3)
        device_id = atol(argv[2]);

    // Compare options and do work
    if (strcmp(argv[1], "SCAN") == 0) {
        // Print Available Devices' id
        uint32_t device_id_array[10], size;
        errcode = get_device_id_array(device_id_array, &size);
        if (errcode != 0) {
            char error_string[150];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] Get Device ID Array: " << error_string << std::endl;
            return -1;
        } else {
            // Print IDs
            for (int i = 0; i < size; i++) {
                std::cout << "Device ID: " << device_id_array[i] << std::endl;
            }
        }
    } else if (argc != 3) {
        goto help_exit;
    } else if (strcmp(argv[1], "RST") == 0) {
        // Reset Device
        errcode = eud_msm_reset(device_id, 100);
        if (errcode != 0) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] Reset Device: " << error_string << std::endl;
            return -1;
        }
    } else if (strcmp(argv[1], "RST_AST") == 0) {
        // Assert Reset
        errcode = eud_msm_assert_reset(device_id);
        if (errcode != 0) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] Assert Reset: " << error_string << std::endl;
            return -1;
        }
    } else if (strcmp(argv[1], "RST_DEAST") == 0) {
        // De-assert Reset
        errcode = eud_msm_deassert_reset(device_id);
        if (errcode != 0) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] De-assert Reset: " << error_string << std::endl;
            return -1;
        }
    } else if (strcmp(argv[1], "SWD") == 0) {
        // Init SWD
        JtagEudDevice *jtag_handle = eud_initialize_device_jtag(device_id, 1, &errcode);
        if (jtag_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize JTAG device: " << error_string << std::endl;
            return -1;
        }
        /* Wait JTAG Device available */
        SwdEudDevice *swd_handle = eud_initialize_device_swd(device_id, 0, &errcode);
        if (swd_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize SWD device: " << error_string << std::endl;
            return -1;
        }
        eud_close_peripheral((PVOID *) swd_handle);
    } else if (strcmp(argv[1], "JTAG") == 0) {
        // Init JTAG
        JtagEudDevice *jtag_handle = eud_initialize_device_jtag(device_id, 0, &errcode);
        if (jtag_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize JTAG device: " << error_string << std::endl;
            return -1;
        }
        eud_close_peripheral((PVOID *) jtag_handle);
    } else if (strcmp(argv[1], "COM") == 0) {
        // Init COM
        ComEudDevice *com_handle = eud_initialize_device_com(device_id, 0, &errcode);
        if (com_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize COM device: " << error_string << std::endl;
            return -1;
        }
        eud_close_peripheral((PVOID *) com_handle);
    } else if (strcmp(argv[1], "TRACE") == 0) {
        // Init TRACE
        TraceEudDevice *trace_handle = eud_initialize_device_trace(device_id, 0, &errcode);
        if (trace_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize TRACE device: " << error_string << std::endl;
            return -1;
        }
        // end eud
        eud_close_peripheral((PVOID *) trace_handle);
    } else {
        // Option not supported
        help_exit:
        printf(help_msg, argv[0]);
        return -EINVAL;
    }
    return 0;
}