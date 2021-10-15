obj-m 		:= driver_encoder_EMIOgpio_PL.o
PWD 		:= $(shell pwd)

KERNEL_DIR  = /home/martin/Escritorio/Tesis/Petalinux_Projects/Imagen/build/tmp/work/plnx_zynq7-xilinx-linux-gnueabi/linux-xlnx/4.19-xilinx-v2019.1+gitAUTOINC+9811303824-r0/linux-plnx_zynq7-standard-build
#KERNEL_DIR = /home/martin/Escritorio/Tesis/Petalinux_Projects/Imagen/build/tmp/work-shared/plnx-zynq7/kernel-source

#GCC  = arm-xilinx-linux-gnueabi-
GCC = arm-linux-gnueabihf-
#GCC = arm-none-eabi-

all:
	make ARCH=arm CROSS_COMPILE=$(GCC) -C $(KERNEL_DIR) M=$(PWD) modules -Wall

clean:
	make ARCH=arm CROSS_COMPILE=$(GCC) -C $(KERNEL_DIR) M=$(PWD) clean

send:
	scp -r ./Apps ./Drivers ./ServidorPNG root@192.168.100.130:/home/root/Tesis

source:
	cd /tools/Xilinx/Vivado/2019.1/
	source settings64.sh
	cd ~/Escritorio/Tesis/Petalinux_Projects/Drivers
