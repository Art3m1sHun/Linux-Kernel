#toolchain
TOOLCHAIN := /opt/poky/5.0.17/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi

PREFIX := arm-poky-linux-gnueabi-

SYSROOT := /opt/poky/5.0.17/sysroots/cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi

CFLAGS := --sysroot=$(SYSROOT) \
          -mthumb \
          -mfpu=neon-vfpv4 \
          -mfloat-abi=hard \
          -mcpu=cortex-a7

ifeq (${ARCH}, arm)
  CC := $(TOOLCHAIN)/$(PREFIX)gcc
  endif
  
all:
	$(CC) $(CFLAGS) -o hello_app hello_raspi.c
clean:
	rm -rf hello_app
