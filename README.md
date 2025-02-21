# Test EUD

Enable eud devices via libeud.

## Compatible System

- Windows x64

## Build

- Ensure you have installed `ibusb-1.0` on you computer(via vcpkg or others).
- `libeud.dll` is prebuilt in this repo.
```pwsh
mkdir build
cmake -B build -S .
```

## Tips

There are many usb devices in eud, here is a list:

| VID    | PID    | Interface Name   |
|--------|--------|------------------|
| 0x05C6 | 0x9501 | EUD Ctrl Device  |
| 0x05C6 | 0x9502 | EUD Trace Device |
| 0x05C6 | 0x9503 | EUD JTAG Device  |
| 0x05C6 | 0x9504 | EUD SWD Device   |
| 0x05C6 | 0x9505 | EUD Port Device  |

QUIC opensource `libeud` uses libusb to interact with eud devices,
so you have to install `WinUSB`(or `libusbK`) driver on the table.

- You may need to `SCAN` the device id of your eud device before control it.

# License
GPL-2.0 OR BSD 3-clause "New" or "Revised" License
