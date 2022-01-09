xterm -hold -e "sshpass -p root ssh root@192.168.100.130 && 
echo system.bin > /sys/class/fpga_manager/fpga0/firmware &&
insmod Tesis/Drivers/driver_i2c_PS_ov7670.ko && 
insmod Tesis/Drivers/driver_vdma.ko && 
./Tesis/Apps/ControlVdma" &
xterm -hold -e "sshpass -p root ssh root@192.168.100.130 && 
cd Tesis/final/sqzdet_c/ && 
./main.bin" &
xterm -hold -e "sshpass -p root ssh root@192.168.100.130 && 
python test.py" &
xterm -hold -e "sshpass -p root ssh root@192.168.100.130 && 
insmod Tesis/Drivers/driver_MIOgpio_PS.ko && 
insmod Tesis/Drivers/driver_pwm_EMIOgpio_PL.ko && 
insmod Tesis/Drivers/driver_EMIOgpio_PL.ko && 
insmod Tesis/Drivers/driver_i2c_PS_hmc5883L.ko && 
rmmod Tesis/Drivers/driver_i2c_PS_hmc5883L.ko && 
insmod Tesis/Drivers/driver_encoder_EMIOgpio_PL.ko && 
./Tesis/develop/readSTATE.bin" &
xterm -hold -e "sshpass -p root ssh root@192.168.100.130 && 
./Tesis/Apps/usrSpace_top & disown && 
./Tesis/develop/FuzzyControl.bin & disown && 
./Tesis/Apps/usrSpace_drive & disown"