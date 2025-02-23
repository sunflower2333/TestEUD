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
#include <libusb.h>

int main(int argc, char **argv) {
    EUD_ERR_t errcode;
    uint32_t device_id_array[3], size;
    uint32_t *device_id = &device_id_array[0];

    const char *help_msg = "Invalid Parameters. Please check usage.\n"
                           "Usage:\n"
                           "\t%s <options>\n\n"
                           "\tavailable options:\n"
                           "\t\tRST: reset device\n"
                           "\t\tRST_AST: assert reset\n"
                           "\t\tRST_DEAST: de-assert reset\n"
                           "\t\tSWD: enable SWD\n"
                           "\t\tJTAG: enable JTAG\n"
                           "\t\tCOM: enable COM\n"
                           "\t\tTRACE: enable TRACE\n";

    // Parse Args
    if (argc != 2) {
        // Print usage if arg numbers not meet.
        goto help_exit;
    }

    // Scan id
    errcode = get_device_id_array(device_id_array, &size);
    if (errcode != 0) {
        char error_string[150];
        uint32_t string_size;
        eud_get_error_string(errcode, error_string, &string_size);
        std::cerr << "[errcode] Get Device ID Array: " << error_string << std::endl;
        return -1;
    } else {
        // Print IDs
        std::cout << "Device ID: " << device_id_array[0] << std::endl;
    }

    // Compare options and do work
    if (strcmp(argv[1], "RST") == 0) {
        // Reset Device
        errcode = eud_msm_reset(*device_id, 100);
        if (errcode != 0) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] Reset Device: " << error_string << std::endl;
            return -1;
        }
    } else if (strcmp(argv[1], "RST_AST") == 0) {
        // Assert Reset
        errcode = eud_msm_assert_reset(*device_id);
        if (errcode != 0) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] Assert Reset: " << error_string << std::endl;
            return -1;
        }
    } else if (strcmp(argv[1], "RST_DEAST") == 0) {
        // De-assert Reset
        errcode = eud_msm_deassert_reset(*device_id);
        if (errcode != 0) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "[errcode] De-assert Reset: " << error_string << std::endl;
            return -1;
        }
    } else if (strcmp(argv[1], "SWD") == 0) {
        // Init CTL
//        CtlEudDevice *ctl_handle = eud_initialize_device_ctl(*device_id, 0, &errcode);
        // enable jatg & swd
//        init_generic_eud_device(DEVICETYPE_EUD_JTG, ctl_handle, &errcode);
//        init_generic_eud_device(DEVICETYPE_EUD_SWD, ctl_handle, &errcode);
        // close CTL
//        eud_close_peripheral((PVOID *) ctl_handle);

        // Init SWD
        JtagEudDevice *jtag_handle = eud_initialize_device_jtag(*device_id, 1, &errcode);
        if (jtag_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize JTAG device: " << error_string << std::endl;
            return -1;
        }
        SwdEudDevice *swd_handle = eud_initialize_device_swd(*device_id, 0, &errcode);
        if (swd_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize SWD device: " << error_string << std::endl;
            return -1;
        }
        // Read Jtag ID
        uint32_t jtag_id;
        swd_get_jtag_id(swd_handle, &jtag_id);
        printf("Jtag ID: 0x%08X\n", jtag_id);

//        eud_close_peripheral((PVOID *) jtag_handle);
//        eud_close_peripheral((PVOID *) swd_handle);
    } else if (strcmp(argv[1], "JTAG") == 0) {
        // Init JTAG
        JtagEudDevice *jtag_handle = eud_initialize_device_jtag(*device_id, 0, &errcode);
        if (jtag_handle == nullptr) {
            char error_string[200];
            uint32_t string_size;
            eud_get_error_string(errcode, error_string, &string_size);
            std::cerr << "Failed to initialize JTAG device: " << error_string << std::endl;
            return -1;
        }
        // Print Jtag ID
        uint32_t jtag_id;
        jtag_get_jtag_id(jtag_handle, &jtag_id);
        printf("Jtag ID: 0x%08X\n", jtag_id);

        // eud_close_peripheral((PVOID *) jtag_handle);
    } else if (strcmp(argv[1], "COM") == 0) {
        // Init CTL
        CtlEudDevice *ctl_handle = eud_initialize_device_ctl(*device_id, 0, &errcode);
        // enable jatg
        init_generic_eud_device(DEVICETYPE_EUD_COM, ctl_handle, &errcode);
        // close ctl
        eud_close_peripheral((PVOID *) ctl_handle);
        // Init COM
        init_generic_eud_device(DEVICETYPE_EUD_COM, ctl_handle, &errcode);
    } else if (strcmp(argv[1], "TRACE") == 0) {
        // Init TRACE
        TraceEudDevice *trace_handle = eud_initialize_device_trace(*device_id, 0, &errcode);
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