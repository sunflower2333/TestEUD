// Test SWD functions.
#include "utils.h"
#include "lib/inc/arm_dap.h"
#include <iostream>
#include <unistd.h>
#include <libusb.h>
#include <bitset>
#define SWD_AP 1
#define SWD_DP 0
// Define masks and shifts for each field
// Define masks and shifts for each field
#define CDBGPWRUPREQ_SHIFT 28
#define CSYSPWRUPREQ_SHIFT 30

#define ORUNDETECT_MASK   0x1
#define STICKORUN_MASK    0x1 << 1
#define TRNMODE_MASK      0x3 << 2
#define STICKCMP_MASK     0x1 << 4
#define STICKYERR_MASK    0x1 << 5
#define READOK_MASK       0x1 << 6
#define WDATAERR_MASK     0x1 << 7
#define MASKLANE_MASK     0xF << 8
#define TRNCNT_MASK       0xFFF << 12
#define RES0_MASK         0x3 << 24
#define CDBGRSTREQ_MASK   0x1 << 26
#define CDBGRSTACK_MASK   0x1 << 27
#define CDBGPWRUPREQ_MASK 0x1 << 28
#define CDBGPWRUPACK_MASK 0x1 << 29
#define CSYSPWRUPREQ_MASK 0x1 << 30
#define CSYSPWRUPACK_MASK 0x1 << 31

#define EXTRACT_FIELD(value, mask, shift) ((value & mask) >> shift)
// Define macro to set bit fields
#define SET_FIELD(value, mask, shift) (value |= (mask << shift))

int main() {
    // Get first device id
    uint32_t device_id=0, size;
    EUD_ERR_t errcode = get_device_id_array(&device_id, &size);
    check_error("Get device id array: status", errcode);
    if (errcode == EUD_GENERIC_FAILURE)
        return -1;

    // // Get jtag handle
    // JtagEudDevice *jtag_handle = eud_initialize_device_jtag(device_id, 0, &errcode);
    // check_error("Initialize JTAG device: status", errcode);
    // eud_msm_reset(device_id, 100);
    // eud_msm_assert_reset(device_id);
    // get swd handle
    SwdEudDevice *swd_handle = eud_initialize_device_swd(device_id, 0, &errcode);
    check_error("swd init", errcode);
    // swd_set_swd_status_max_count(swd_handle,0);
    // reset the seq
    // assert_reset_sequence(swd_handle);

    // // Get SWD ID
    uint32_t swd_id = 0;
    errcode = swd_get_jtag_id(swd_handle, &swd_id);
    check_error("SWD ID ", errcode);
    printf("DAP jtag ID: 0x%08X\n", swd_id);
    set_buf_mode(swd_handle, 1);

    // Read SWD DP registers
    uint32_t status = 0;
    check_error("swd read: ", swd_read(swd_handle, SWD_DP, 1, &status));

    // Output the status in binary format
    std::cout << "status (binary): " << std::bitset<32>(status) << "\n";

    // Output the results using the macro directly in cout
    // std::cout << "orundetect: " << EXTRACT_FIELD(status, ORUNDETECT_MASK, 0) << "\n"
    //           << "stickorun: " << EXTRACT_FIELD(status, STICKORUN_MASK, 1) << "\n"
    //           << "trnmode: " << EXTRACT_FIELD(status, TRNMODE_MASK, 2) << "\n"
    //           << "stickcmp: " << EXTRACT_FIELD(status, STICKCMP_MASK, 4) << "\n"
    //           << "stickyerr: " << EXTRACT_FIELD(status, STICKYERR_MASK, 5) << "\n"
    //           << "readok: " << EXTRACT_FIELD(status, READOK_MASK, 6) << "\n"
    //           << "wdataerr: " << EXTRACT_FIELD(status, WDATAERR_MASK, 7) << "\n"
    //           << "masklane: " << EXTRACT_FIELD(status, MASKLANE_MASK, 8) << "\n"
    //           << "trncnt: " << EXTRACT_FIELD(status, TRNCNT_MASK, 12) << "\n"
    //           << "res0: " << EXTRACT_FIELD(status, RES0_MASK, 24) << "\n"
    //           << "cdbgrstreq: " << EXTRACT_FIELD(status, CDBGRSTREQ_MASK, 26) << "\n"
    //           << "cdbgrstack: " << EXTRACT_FIELD(status, CDBGRSTACK_MASK, 27) << "\n"
    //           << "cdbgpwrupreq: " << EXTRACT_FIELD(status, CDBGPWRUPREQ_MASK, 28) << "\n"
    //           << "cdbgpwrupack: " << EXTRACT_FIELD(status, CDBGPWRUPACK_MASK, 29) << "\n"
    //           << "csyspwrupreq: " << EXTRACT_FIELD(status, CSYSPWRUPREQ_MASK, 30) << "\n"
    //           << "csyspwrupack: " << EXTRACT_FIELD(status, CSYSPWRUPACK_MASK, 31)
    //           << std::endl;

    // Set CDBGPWRUPREQ & CSYSPWRUPREQ
    SET_FIELD(status, 0x1, CDBGPWRUPREQ_SHIFT);
    SET_FIELD(status, 0x1, CSYSPWRUPREQ_SHIFT);

    swd_write(swd_handle, SWD_DP, 1, status);
    // check_error("swd set status: ", swd_write(swd_handle, SWD_DP, 1, status));
    /* Wait done */
    Sleep(100);
    // Flush Okay & read again
    status = 0;
    check_error("swd read: ", swd_read(swd_handle, SWD_DP, 1, &status)); // 0x2 -> 0x8
    std::cout << "status (binary): " << std::bitset<32>(status) << "\n";

    uint32_t data=0;
    // for (int i = 0; i < 0xFF'FF'FF'FF; i++) {
// while (1) {
    swd_write(swd_handle, SWD_DP, 2, 0xF0);
    swd_read(swd_handle, SWD_AP, 3, &data);
    swd_read(swd_handle, SWD_AP, 3, &data);
    // swd_read(swd_handle, SWD_AP, 3, &data);
    printf("addr: 0x%08X data: 0x%08X\n", 0xF0, data);

    swd_write(swd_handle, SWD_DP, 2, 0x00);
    swd_read(swd_handle, SWD_AP, 3, &data);
    swd_read(swd_handle, SWD_AP, 3, &data);
    // swd_flush(swd_handle);
    // swd_read(swd_handle, SWD_AP, 3, &data);
    printf("addr: 0x%08X data: 0x%08X\n", 0x00, data);
// }
    // swd_write(swd_handle, SWD_DP, 0, 0b11110000000000000000000000000000);

    // }
    // Use SWD Write to Select AP0
    /*
    set_buf_mode(swd_handle, 0);

    for (int i = 0; i < 0xFF; i++) {
        errcode = swd_write(swd_handle, SWD_DP, 0b10, i);
        swd_flush(swd_handle);
        // check_error("SWD Write ", errcode);
        uint32_t idr = 0;
        errcode = swd_read(swd_handle, SWD_AP, 0b10, &idr);
        // check_error("SWD Read ", errcode);
        if (idr != 0)
            printf("IDR: 0x%08X\n", idr);
        errcode = swd_read(swd_handle, SWD_AP, 0b01, &idr);
        // check_error("SWD Read ", errcode);
        if (idr != 0)
            printf("IDR: 0x%08X\n", idr);
        errcode = swd_read(swd_handle, SWD_AP, 0b00, &idr);
        // check_error("SWD Read ", errcode);
        if (idr != 0)
            printf("IDR: 0x%08X\n", idr);
        errcode = swd_read(swd_handle, SWD_AP, 0b11, &idr);
        // check_error("SWD Read ", errcode);
        if (idr != 0)
            printf("IDR: 0x%08X\n", idr);
    }
    */
    // Set swd mode to 2
    // errcode = set_buf_mode(swd_handle,IMMEDIATEWRITEMODE);

    // deassert_reset_sequence(swd_handle);

    // Set  swd_set_swd_status_max_count
    // swd_set_swd_status_max_count(swd_handle, 0x10);

    // Get swd status
    // uint32_t status=0;
    // check_error("SWD Status status ", swd_get_status(swd_handle, 1, &status));
    // printf("status: %x\n", status);


    // release swd
    // eud_close_peripheral((PVOID *) jtag_handle);
    // eud_close_peripheral((PVOID *) swd_handle);
}
