obj-m := yqs_char_driver.o

CURRENT_PATH:=$(shell pwd)
LINUX_KERNEL:=$(shell uname -r)
LINUX_KERNEL_PATH:=/usr/src/linux-headers-${LINUX_KERNEL}

all:
	echo "LINUX_KERNEL_PATH=${LINUX_KERNEL_PATH}"
	make -C ${LINUX_KERNEL_PATH} M=${CURRENT_PATH} modules

clean:
	rm -rf *.ko *.mod *.mod.c *.o *.order *.symvers
