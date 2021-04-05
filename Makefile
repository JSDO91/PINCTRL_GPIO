obj-m += gpio_platform_driver.o

PWD := $(shell pwd)
 
#KDIR := /home/rpi/Work/linux
KDIR := /root/Work/linux
 
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KDIR) M=$(PWD) modules

clean:
	clear
	make -C $(KDIR) M=$(PWD) clean
	rm  -f $(SRC_PATH)/*.o
