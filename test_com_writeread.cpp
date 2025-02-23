#include <libusb.h>
#include <iostream>
#include <windows.h>

// hex dump an array with char in left
void hexdump(const unsigned char *buffer, unsigned long length) {
    for (unsigned long i = 0; i < length; i++) {
        if (i % 16 == 0) {
            if (i != 0)
                std::cout << std::endl;
        }
        std::cout << std::hex << (int) buffer[i] << " ";
    }
    std::cout << std::endl;
}

// Write Device
void write_device(libusb_device_handle *handle, unsigned char *data, int length) {
    int actual_length;
    int result = libusb_bulk_transfer(handle, 0x02, data, length, &actual_length, 0);
    if (result == 0 && actual_length == length) {
        std::cout << "Data written successfully: " << 5 << std::endl;
    } else {
        std::cerr << "Error writing data: " << libusb_error_name(result) << std::endl;
    }
}
// Read device
void read_device(libusb_device_handle *handle, unsigned char *data, int length) {
    int actual_length;
    int result = libusb_bulk_transfer(handle, 0x81, data, length, &actual_length, 0);
    if (result == 0 && actual_length == length) {
        std::cout << "Data read successfully: " << 5 << std::endl;
    } else {
        std::cerr << "Error reading data: " << libusb_error_name(result) << std::endl;
    }
}

int main(){
    int err;

    libusb_context *ctx = NULL;

    // Initialize libusb
    err = libusb_init(&ctx);
    if (err < 0) {
        std::cerr << "Failed to initialize libusb: " << libusb_error_name(err) << std::endl;
        return 1;
    }

//    libusb_set_debug(ctx, 3);

    // Get handle by pid/vid
    libusb_device_handle *handle = libusb_open_device_with_vid_pid(ctx, 0x05C6, 0x9505);
    // Validate handle
    if (!handle) {
        std::cerr << "Error opening device" << std::endl;
        libusb_exit(ctx);
        return 1;
    }

    // Claim Interface
    if (libusb_claim_interface(handle, 0) < 0) {
        std::cerr << "Cannot claim interface" << std::endl;
        libusb_close(handle);
        libusb_exit(ctx);
        return 1;
    }

    // Write something to test handle
    uint8_t buf1[2] = {0};
    write_device(handle, buf1, sizeof(buf1));

    // write 0x1ffff00
    uint8_t buf3[5] = {0x1, 0xff, 0xff, 0x00, 0x00};
    write_device(handle, buf3, sizeof(buf3));

    // write 0x2ffff00
    uint8_t buf4[5] = {0x2, 0xff, 0xff, 0x00, 0x00};
    write_device(handle, buf4, sizeof(buf3));

    // write
    uint8_t buf5[5] = {0x81, 5, 0x00, 0x00, 0x00};
    write_device(handle, buf5, sizeof(buf5));


    // read something
    uint8_t in_buf[16] = {0};
    read_device(handle, in_buf, sizeof(in_buf));

    // print data in msg
    hexdump(in_buf, sizeof(in_buf));


    // 释放接口
    libusb_release_interface(handle, 0);

    // 关闭设备
    libusb_close(handle);
    libusb_exit(ctx);
}




//int main() {
//    // Device Path
//#define SERIAL_PATH "//./COM16"
//    // 打开COM端口
//    HANDLE hSerial = CreateFile(
//            SERIAL_PATH,
//            GENERIC_READ | GENERIC_WRITE,
//            0,
//            NULL,
//            OPEN_EXISTING,
//            FILE_ATTRIBUTE_NORMAL,
//            NULL
//    );
//
//    if (hSerial == INVALID_HANDLE_VALUE) {
//        std::cerr << "Error opening COM port" << std::endl;
//        return 1;
//    }
//
//    // 配置串行端口参数
//    DCB dcbSerialParams = {0};
//    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
//
//    if (!GetCommState(hSerial, &dcbSerialParams)) {
//        std::cerr << "Error getting COM port state" << std::endl;
//        CloseHandle(hSerial);
//        return 1;
//    }
//
//    dcbSerialParams.BaudRate = CBR_115200;   // 设置波特率
//    dcbSerialParams.ByteSize = 8;          // 设置数据位
//    dcbSerialParams.StopBits = ONESTOPBIT; // 设置停止位
//    dcbSerialParams.Parity = NOPARITY;     // 设置校验位
//
//    if (!SetCommState(hSerial, &dcbSerialParams)) {
//        std::cerr << "Error setting COM port state" << std::endl;
//        CloseHandle(hSerial);
//        return 1;
//    }
//
//    // 设置超时参数
////    COMMTIMEOUTS timeouts = {0};
////    timeouts.ReadIntervalTimeout = 50;
////    timeouts.ReadTotalTimeoutConstant = 50;
////    timeouts.ReadTotalTimeoutMultiplier = 10;
////
////    if (!SetCommTimeouts(hSerial, &timeouts)) {
////        std::cerr << "Error setting COM port timeouts" << std::endl;
////        CloseHandle(hSerial);
////        return 1;
////    }
//
//    // 写入配置信息
//    uint8_t* tx_timeout = new uint8_t[4];
//
//    *tx_timeout = 0xFF;
//    *(tx_timeout+1) = 0xFF;
//    *(tx_timeout + 2) = 0x0;
//    *(tx_timeout + 3) = 0x0;
//
//    // 发送
//    DWORD dataSent;
//    bool writeSuccess =  WriteFile(hSerial, tx_timeout, 4, &dataSent, NULL);
//
//    if (writeSuccess) {
//        std::cout << "Data written successfully: " << dataSent << std::endl;
//    } else {
//        std::cerr << "Error writing data" << std::endl;
//    }
//
//    uint8_t* rx_timeout = new uint8_t[4];
//    rx_timeout[0] = 0xFF;
//    rx_timeout[1] = 0xFF;
//    rx_timeout[2] = 0x0;
//    rx_timeout[3] = 0x0;
//
//    // timeout set
//    writeSuccess =  WriteFile(hSerial, tx_timeout, 4, &dataSent, NULL);
//
//    if (writeSuccess) {
//        std::cout << "Data written successfully: " << dataSent << std::endl;
//    } else {
//        std::cerr << "Error writing data" << std::endl;
//    }
//
//    // Set poll freq
//    uint8_t* sendbuf = new uint8_t[3];
//    *sendbuf = 0x81;
//    *(sendbuf+1) = 1;
//    *(sendbuf+2) = 13; //Carriage return
//    uint32_t poll_value = 100; //100 milliseconds
//    WriteFile(hSerial, sendbuf, 3, &dataSent, NULL);
//
//
//    uint32_t counter;
//    uint8_t in_data[16] = {0};
//    uint8_t out_data[16] = {0};
//    BOOL data_ready = FALSE;
//    uint32_t len = 0;
//
//    for (counter = 0; counter < 1000; counter++){
//        len = 10;
//        //buffer = assemble_send_message(EE_ID, msglen, defaultmsg);
//        out_data[0] = 0x81; //env ID
//        out_data[1] = len-2; //length of message (1)
//        out_data[2] = 13; //carriage return
//        out_data[3] = 14; //carriage return
//        out_data[4] = 15; //carriage return
//        out_data[5] = 16; //carriage return
//        out_data[6] = 17; //carriage return
//        out_data[7] = 18; //carriage return
//        out_data[8] = 19; //carriage return
//        out_data[9] = 20; //carriage return
//
//        //memcpy(com_device_handle->usb_buffer_out_,
//        //      sendbuf,
//        //      3
//        // );
//
//        // Write To File
//        writeSuccess =  WriteFile(hSerial, out_data, 10, &dataSent, NULL);
//        // Check error
//        if (writeSuccess) {
//            std::cout << "Data written successfully: " << dataSent << std::endl;
//        } else {
//            std::cerr << "Error writing data" << std::endl;
//        }
//
//        // Read File
//        DWORD bytesRead;
//        bool readSuccess = ReadFile(hSerial, in_data, 16, &bytesRead, NULL);
//        // Check error
//        if (readSuccess) {
//            std::cout << "Data read successfully: " << bytesRead << std::endl;
//        } else {
//            std::cerr << "Error reading data" << std::endl;
//        }
//
//        //Sleep for poll_value if no data is ready.
//        // if (data_ready == FALSE) Sleep(100);
//        Sleep(10);
//        // Print MSG in in_data
//        hexdump(in_data, 16);
//    }
//
//
//    // 关闭COM端口
//    CloseHandle(hSerial);
//
//    return 0;
//}
