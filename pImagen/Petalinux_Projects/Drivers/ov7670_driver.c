#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <media/v4l2-subdev.h>

/* Defines */
#define I2C_BUS             0
#define OV7670_I2C_ADDRESS  0x42
#define DEVICE_NAME         "ov7670"
#define BASE_MINOR          0
#define MINOR_COUNT         1


/* Registers */
#define REG_GAIN	        0x00	/* Gain lower 8 bits (rest in vref) */
#define REG_BLUE	        0x01	/* blue gain */
#define REG_RED		        0x02	/* red gain */
#define REG_VREF	        0x03	/* Pieces of GAIN, VSTART, VSTOP */
#define REG_COM1	        0x04	/* Control 1 */
#define COM1_CCIR656	    0x40    /* CCIR656 enable */
#define REG_BAVE	        0x05	/* U/B Average level */
#define REG_GbAVE	        0x06	/* Y/Gb Average level */
#define REG_AECHH	        0x07	/* AEC MS 5 bits */
#define REG_RAVE	        0x08	/* V/R Average level */
#define REG_COM2	        0x09	/* Control 2 */
#define COM2_SSLEEP	        0x10	/* Soft sleep mode */
#define REG_PID		        0x0a	/* Product ID MSB */
#define REG_VER		        0x0b	/* Product ID LSB */
#define REG_COM3	        0x0c	/* Control 3 */
#define COM3_SWAP	        0x40	/* Byte swap */
#define COM3_SCALEEN	    0x08	/* Enable scaling */
#define COM3_DCWEN	        0x04	/* Enable downsamp/crop/window */
#define REG_COM4	        0x0d	/* Control 4 */
#define REG_COM5	        0x0e	/* All "reserved" */
#define REG_COM6	        0x0f	/* Control 6 */
#define REG_AECH	        0x10	/* More bits of AEC value */
#define REG_CLKRC	        0x11	/* Clocl control */
#define CLK_EXT	            0x40	/* Use external clock directly */
#define CLK_SCALE	        0x3f	/* Mask for internal clock scale */
#define REG_COM7	        0x12	/* Control 7 */
#define COM7_RESET	        0x80	/* Register reset */
#define COM7_FMT_MASK	    0x38
#define COM7_FMT_VGA	    0x00
#define	COM7_FMT_CIF	    0x20	/* CIF format */
#define COM7_FMT_QVGA	    0x10	/* QVGA format */
#define COM7_FMT_QCIF	    0x08	/* QCIF format */
#define	COM7_RGB	        0x04	/* bits 0 and 2 - RGB format */
#define	COM7_YUV	        0x00	/* YUV */
#define	COM7_BAYER	        0x01	/* Bayer format */
#define	COM7_PBAYER	        0x05	/* "Processed bayer" */
#define REG_COM8	        0x13	/* Control 8 */
#define COM8_FASTAEC	    0x80	/* Enable fast AGC/AEC */
#define COM8_AECSTEP	    0x40	/* Unlimited AEC step size */
#define COM8_BFILT	        0x20	/* Band filter enable */
#define COM8_AGC	        0x04	/* Auto gain enable */
#define COM8_AWB	        0x02	/* White balance enable */
#define COM8_AEC	        0x01	/* Auto exposure enable */
#define REG_COM9	        0x14	/* Control 9  - gain ceiling */
#define REG_COM10	        0x15	/* Control 10 */
#define COM10_HSYNC	        0x40	/* HSYNC instead of HREF */
#define COM10_PCLK_HB	    0x20	/* Suppress PCLK on horiz blank */
#define COM10_HREF_REV      0x08	/* Reverse HREF */
#define COM10_VS_LEAD	    0x04	/* VSYNC on clock leading edge */
#define COM10_VS_NEG	    0x02	/* VSYNC negative */
#define COM10_HS_NEG	    0x01	/* HSYNC negative */
#define REG_HSTART	        0x17	/* Horiz start high bits */
#define REG_HSTOP	        0x18	/* Horiz stop high bits */
#define REG_VSTART	        0x19	/* Vert start high bits */
#define REG_VSTOP	        0x1a	/* Vert stop high bits */
#define REG_PSHFT	        0x1b	/* Pixel delay after HREF */
#define REG_MIDH	        0x1c	/* Manuf. ID high */
#define REG_MIDL	        0x1d	/* Manuf. ID low */
#define REG_MVFP	        0x1e	/* Mirror / vflip */
#define MVFP_MIRROR	        0x20	/* Mirror image */
#define MVFP_FLIP	        0x10	/* Vertical flip */
#define REG_AEW		        0x24	/* AGC upper limit */
#define REG_AEB		        0x25	/* AGC lower limit */
#define REG_VPT		        0x26	/* AGC/AEC fast mode op region */
#define REG_HSYST	        0x30	/* HSYNC rising edge delay */
#define REG_HSYEN	        0x31	/* HSYNC falling edge delay */
#define REG_HREF	        0x32	/* HREF pieces */
#define REG_TSLB	        0x3a	/* lots of stuff */
#define TSLB_YLAST	        0x04	/* UYVY or VYUY - see com13 */
#define REG_COM11	        0x3b	/* Control 11 */
#define COM11_NIGHT	        0x80	/* NIght mode enable */
#define COM11_NMFR	        0x60    /* Two bit NM frame rate */
#define COM11_HZAUTO	    0x10    /* Auto detect 50/60 Hz */
#define	COM11_50HZ	        0x08	/* Manual 50Hz select */
#define COM11_EXP	        0x02
#define REG_COM12	        0x3c	/* Control 12 */
#define COM12_HREF	        0x80	/* HREF always */
#define REG_COM13	        0x3d	/* Control 13 */
#define COM13_GAMMA	        0x80	/* Gamma enable */
#define	COM13_UVSAT	        0x40	/* UV saturation auto adjustment */
#define COM13_UVSWAP	    0x01	/* V before U - w/TSLB */
#define REG_COM14	        0x3e	/* Control 14 */
#define COM14_DCWEN	        0x10	/* DCW/PCLK-scale enable */
#define REG_EDGE	        0x3f	/* Edge enhancement factor */
#define REG_COM15	        0x40	/* Control 15 */
#define COM15_R10F0	        0x00	/* Data range 10 to F0 */
#define	COM15_R01FE	        0x80	/*            01 to FE */
#define COM15_R00FF	        0xc0	/*            00 to FF */
#define COM15_RGB565	    0x10	/* RGB565 output */
#define COM15_RGB555	    0x30	/* RGB555 output */
#define REG_COM16	        0x41	/* Control 16 */
#define COM16_AWBGAIN       0x08	/* AWB gain enable */
#define REG_COM17	        0x42    /* Control 17 */
#define COM17_AECWIN	    0xc0	/* AEC window - must match COM4 */
#define COM17_CBAR	        0x08	/* DSP Color bar */

static struct state {
    struct i2c_client *ov7670_client;

    dev_t devtype;          // numero mayor y menor
    struct class *devclass; // class_create()
    struct device *dev;     // device_create()
    struct cdev chardev;    // cdev_add()
} state;


struct regval_list {
	unsigned char reg_num;
	unsigned char value;
};


static struct i2c_board_info ov7670_i2c_board_info = {
    I2C_BOARD_INFO("ov7670", OV7670_I2C_ADDRESS)
};


static struct regval_list ov7670_default_regs[] = {
    { 0x12, 0x80 }, // COM7   Reset
	{ 0x12, 0x80 }, // COM7   Reset
	{ 0x12, 0x04 }, // COM7   Size & RGB output
	{ 0x11, 0x40 }, // CLKRC  Prescaler - Fin/(1+1)
	{ 0x0C, 0x00 }, // COM3   Lots of stuff, enable scaling, all others off
	{ 0x3E, 0x00 }, // COM14  PCLK scaling off
   	{ 0x8C, 0x00 }, // RGB444 Set RGB format
   	{ 0x04, 0x00 }, // COM1   no CCIR601
 	{ 0x40, 0x10 }, // COM15  Full 0-255 output, RGB 565
	{ 0x3a, 0x04 }, // TSLB   Set UV ordering,  do not auto-reset window
	{ 0x14, 0x38 }, // COM9  - AGC Celling
	{ 0x4f, 0xb3 }, // MTX1  - colour conversion matrix
	{ 0x50, 0xb3 }, // MTX2  - colour conversion matrix
	{ 0x51, 0x00 }, // MTX3  - colour conversion matrix
	{ 0x52, 0x3d }, // MTX4  - colour conversion matrix
	{ 0x53, 0xa7 }, // MTX5  - colour conversion matrix
	{ 0x54, 0xe4 }, // MTX6  - colour conversion matrix
	{ 0x58, 0x9e }, // MTXS  - Matrix sign and auto contrast
	{ 0x3d, 0xc0 }, // COM13 - Turn on GAMMA and UV Auto adjust
	{ 0x11, 0x00 }, // CLKRC  Prescaler - Fin/(1+1)
	{ 0x17, 0x11 }, // HSTART HREF start (high 8 bits)
	{ 0x18, 0x61 }, // HSTOP  HREF stop (high 8 bits)
	{ 0x32, 0xA4 }, // HREF   Edge offset and low 3 bits of HSTART and HSTOP
	{ 0x19, 0x03 }, // VSTART VSYNC start (high 8 bits)
	{ 0x1A, 0x7b }, // VSTOP  VSYNC stop (high 8 bits) 
	{ 0x03, 0x0a }, // VREF   VSYNC low two bits
    { 0x0e, 0x61 }, // COM5(0x0E) 0x61
    { 0x0f, 0x4b }, // COM6(0x0F) 0x4B 
    { 0x16, 0x02 }, //
    { 0x1e, 0x37 }, // MVFP (0x1E) 0x07  -- FLIP AND MIRROR IMAGE 0x3x
    { 0x21, 0x02 },
    { 0x22, 0x91 },
    { 0x29, 0x07 },
    { 0x33, 0x0b },   
    { 0x35, 0x0b },
    { 0x37, 0x1d },             
    { 0x38, 0x71 },
    { 0x39, 0x2a },             
    { 0x3c, 0x78 }, // COM12 (0x3C) 0x78
    { 0x4d, 0x40 },                   
    { 0x4e, 0x20 },
    { 0x69, 0x00 }, // GFIX (0x69) 0x00                 
    { 0x6b, 0x4a },
    { 0x74, 0x10 }, 
    { 0x8d, 0x4f },
    { 0x8e, 0x00 },                   
    { 0x8f, 0x00 },
    { 0x90, 0x00 },                      
    { 0x91, 0x00 },
    { 0x96, 0x00 },              
    { 0x9a, 0x00 },
    { 0xb0, 0x84 },          
    { 0xb1, 0x0c },
    { 0xb2, 0x0e },
    { 0xb3, 0x82 },
    { 0xb8, 0x0a },
	{ 0xFF, 0xFF }, //mark end of ROM
};


/*************************************************************************************************/
/******************************** Estructura file_operations *************************************/
/**
    @brief Implementacion de las funciones del driver open, read, write, close, ioctl, cada driver
    tiene su propia file_operations
**/

static const struct file_operations driver_file_op = {
  //.owner = THIS_MODULE,
  //.open = driver_open,
  //.read = driver_read,
  //.release = driver_release,
  //.unlocked_ioctl = driver_ioctl,
};
/*************************************************************************************************/
/*************************************************************************************************/

static int ov7670_write_i2c(struct i2c_client *client, unsigned char reg, unsigned char value)
{
	//struct i2c_client *client = v4l2_get_subdevdata(sd);
    pr_info("ov7670_write_i2c: %08X, %08X !\n", reg, value);

	struct i2c_msg msg;
	unsigned char data[2] = { reg, value };
	int ret;

	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = 2;
	msg.buf = data;
	ret = i2c_transfer(client->adapter, &msg, 1);
	if (ret > 0)
		ret = 0;
	if (reg == REG_COM7 && (value & COM7_RESET))
		msleep(5);  /* Wait for reset to run */
	return ret;
}

static int ov7670_write(struct i2c_client *client, unsigned char reg, unsigned char value)
{
	//struct ov7670_info *info = to_state(client);

	ov7670_write_i2c(client, reg, value);
    return 0;
}

/*
 * Write a list of register settings; ff/ff stops the process.
 */
static int ov7670_write_array(struct i2c_client *client, struct regval_list *vals)
{
	while (vals->reg_num != 0xff || vals->value != 0xff) {
		int ret = ov7670_write(client, vals->reg_num, vals->value);

		if (ret < 0)
			return ret;
		vals++;
	}
	return 0;
}


static int ov7670_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct state *dev; 

    pr_info("ov7670_probe: Hola, entre a probe!\n");

    pr_info("my_pdrv_init: Intento allocar cdev\n");
    if (alloc_chrdev_region(&state.devtype, BASE_MINOR, MINOR_COUNT, DEVICE_NAME) != 0) 
    {
        pr_err(DEVICE_NAME": alloc_chrdev_region: Error no es posible asignar numero mayor\n");
        return -EBUSY;  // Device or resource busy 
    }
    pr_info("my_pdrv_init: Cdev struct allocada\n");


    /* Procedemos a registrar el dispositivo en el sistema */
    pr_info("my_pdrv_init: Procedo a registrar el dispositivo en el sistema\n");

    /* Para inicializarla se puede inicializar a mano o bien una vez definida file_operations...  */
    pr_info("%my_pdrv_init: Cargo cdev struct\n");
    cdev_init(&state.chardev, &driver_file_op);

    /* Para agregarla al sistema */
    pr_info("my_pdrv_init: Procedo a agregar la estructura cdev al sistema\n");
    if (cdev_add(&state.chardev, state.devtype, MINOR_COUNT) != 0) 
    {
        pr_err(DEVICE_NAME": cdev_add: No se pude agregar el cdev\n");
        unregister_chrdev_region(state.devtype, MINOR_COUNT);
        return -EPERM;    // -1
    }
    pr_info("my_pdrv_init: Estructura cdev agregada al sistema\n");


    if (client->addr != id->driver_data) {
        pr_info("ov7670_probe: Direccion equivada (es: %d)!\n", client->addr);
        return -ENODEV;
    }

    memset(&state, 0, sizeof(state));
    dev = &state; 
    i2c_set_clientdata(client, dev);

    state.ov7670_client = client;

    pr_info("ov7670_probe: Configuro ov7670!\n");
    ov7670_write_array(client, ov7670_default_regs);
    pr_info("ov7670_probe: Termine!\n");

    return 0;
}

static int ov7670_remove(struct i2c_client *client)
{
    struct usense_device_descr *dev = i2c_get_clientdata(client);
    pr_info("Hola! Entre a ov7670_remove! \n");

    if(dev){
        i2c_set_clientdata(client, NULL);

        pr_info("my_pdrv_remove: Quito cdev del sistema!\n");
        cdev_del(&state.chardev);       
        pr_info("my_pdrv_remove: Desaloco cdev!\n");
        unregister_chrdev_region(state.devtype, MINOR_COUNT);
    }

    return 0;
}

// Generalmente se implementa una unica estructura del contolador 
// y se instancian todos los clientes en ella. ej. "ov7670". 

static struct i2c_device_id ov7670_idtable[] = {
	{ "ov7670", OV7670_I2C_ADDRESS },
	{ }
};


MODULE_DEVICE_TABLE(i2c, ov7670_idtable);

static struct i2c_driver ov7670_driver = {
	.driver = {
		.name	= "ov7670",
        .owner = THIS_MODULE,
	},

	.id_table	= ov7670_idtable,
	.probe		= ov7670_probe,
	.remove		= ov7670_remove,
};

//module_i2c_driver(ov7670_driver);


static int __init my_i2cdriver_init (void) 
{
    struct i2c_adapter *adapter = i2c_get_adapter(I2C_BUS);
    int ret;

    pr_info("my_i2cdriver_init: Hola mundo!\n");

    if(!adapter) {
        pr_info("my_i2cdriver_init: Error consiguiendo el adaptador! (fue el: %d)\n", I2C_BUS);
        return -ENODEV;
    }

    pr_info("my_i2cdriver_init: Creo nuevo device!\n");
    state.ov7670_client = i2c_new_device(adapter, &ov7670_i2c_board_info);

    if (!state.ov7670_client) {
        pr_info("i2c_new_device: Creando nuevo dispositivo!\n");
        i2c_put_adapter(adapter);
        return -1;
    }

    ret = i2c_add_driver(&ov7670_driver);
    if (ret < 0) {
        pr_info("i2c_add_driver: Agregando nuevo dispositivo!\n");
        i2c_unregister_device(state.ov7670_client);
        return ret;
    }

    return 0;
}
module_init(my_i2cdriver_init);

static void __exit my_i2cdriver_remove (void)    // se ejecuta cuando hago rmmod
{
    i2c_del_driver(&ov7670_driver);
    i2c_unregister_device(state.ov7670_client);
}
module_exit(my_i2cdriver_remove);

/*************************************************************************************************/
/************************************ Module Information *****************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Martin Fuschetto");
MODULE_DESCRIPTION("My i2c device ov7670 driver");

