<h1 align="center"> Implementacion sobre la Zybo </h1> 

## Desarrollos

* [Linux-VDMA-TPG-PNG.Server](https://github.com/Fuschetto97/Tesis/blob/main/Petalinux_Projects/README_Des1.md),  Tiempo = 2 semanas
* [Linux-VDMA-ov7670(camara)-PNG.Server](https://github.com/Fuschetto97/Tesis/blob/main/Petalinux_Projects/README_Des2.md) Tiempo =  5 dias
* [I2C controller handling SCCB](https://github.com/Fuschetto97/Tesis/blob/main/Petalinux_Projects/README_Des3.md), Tiempo =  5 dias
* Drivers para los motores, Tiempo =  2 dias

## Diagrama en bloques 

<img src=./imagenes/BlockDiagram.png alt="Hardware" />

## Diagrama en Vivado 

<img src=./imagenes/Vivado2.png alt="Hardware" />

## Petalinux-build

* petalinux-config --get-hw-description /home/martin/Escritorio/Tesis/pImagen/Petalinux_Projects/Imagen/

* petalinux-config -c kernel

* petalinux-config -c rootfs

* petalinux-build

* petalinux-package --boot --fsbl /home/martin/Escritorio/Tesis/pImagen/Petalinux_Projects/Imagen/images/linux/zynq_fsbl.elf --fpga /home/martin/Escritorio/Tesis/pImagen/Petalinux_Projects/Imagen/system.bit --uboot












