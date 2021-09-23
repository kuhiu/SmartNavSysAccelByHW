#Recordar correr el gcc desde tool/xilinx

#squeezedet:
#	@echo Generando $@.o...
#	arm-linux-gnueabihf-gcc -Ofast -mfpu=neon -mcpu=cortex-a9 -ffast-math -o main.bin main.c fun.c -Wall -lm -lpng 

squeezedet:
	@echo Generando $@.o...
	arm-linux-gnueabihf-gcc -Ofast -mfpu=neon -mcpu=cortex-a9 -ffast-math -o main.bin main.c fun.c -Wall -lm -lpng 

asm:
	arm-linux-gnueabihf-gcc -Ofast -mfpu=neon -mcpu=cortex-a9 -ffast-math -S fun.c