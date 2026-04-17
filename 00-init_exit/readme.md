# lệnh tải linux-headers
    sudo apt install -y linux-headers-`uname -r`
# load module
    sudo insmod "tên module.ko"
# Xóa module
    sudo rmmod "tên module"
# xem bản header-file
    uname -r
# xem log
    dmesg | tail
# module information
    modinfo "tên module"
    License: Giấy phép GPL
    Author: Nhà phát triển module
    Module description: Mô tả sơ bộ về module
    Module version: phiên bản module