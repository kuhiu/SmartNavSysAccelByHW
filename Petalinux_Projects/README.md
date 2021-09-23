<h1 align="center"> Modulo: Procesamiento de imagenes </h1> 

## Desarrollos

* [Linux-VDMA-TPG-PNG.Server](https://github.com/Fuschetto97/Tesis/blob/main/Petalinux_Projects/README_Des1.md)  Tiempo = 2 semanas
* [Linux-VDMA-ov7670(camara)-PNG.Server](https://github.com/Fuschetto97/Tesis/blob/main/Petalinux_Projects/README_Des2.md) Tiempo =  5 dias
* [I2C controller handling SCCB](https://github.com/Fuschetto97/Tesis/blob/main/Petalinux_Projects/README_Des3.md) Tiempo =  5 dias


## Petalinux-build

* petalinux-config --get-hw-description /home/martin/Escritorio/Tesis/pImagen/Petalinux_Projects/Imagen/

* petalinux-config -c kernel

* petalinux-config -c rootfs

* petalinux-build

* petalinux-package --boot --fsbl /home/martin/Escritorio/Tesis/pImagen/Petalinux_Projects/Imagen/images/linux/zynq_fsbl.elf --fpga /home/martin/Escritorio/Tesis/pImagen/Petalinux_Projects/Imagen/system.bit --uboot












