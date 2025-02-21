cmake_minimum_required(VERSION 3.28)
project(EUDTestcases)

set(CMAKE_CXX_STANDARD 17)
set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/inc)
add_link_options(-static)

INCLUDE_DIRECTORIES(
        D:/VcPkg/clion/installed/x64-windows/include
)
LINK_DIRECTORIES(
        D:/VcPkg/clion/installed/x64-windows/lib/
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libusb REQUIRED IMPORTED_TARGET libusb-1.0)

add_executable(EUDTestcases main.cpp)

# Link the precompiled library
target_link_libraries(EUDTestcases PRIVATE ${LIBRARY_DIR}/libeud.a)

target_link_libraries(EUDTestcases PRIVATE PkgConfig::libusb)