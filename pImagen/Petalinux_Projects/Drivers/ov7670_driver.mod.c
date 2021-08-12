#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xfb080ece, "module_layout" },
	{ 0xd543aa38, "i2c_del_driver" },
	{ 0xbb7fd156, "i2c_unregister_device" },
	{ 0x3124c786, "i2c_register_driver" },
	{ 0x5ad2433, "i2c_put_adapter" },
	{ 0x5857ad53, "i2c_new_device" },
	{ 0x355d00b8, "i2c_get_adapter" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0xf9a482f9, "msleep" },
	{ 0x183ff70a, "i2c_transfer" },
	{ 0x5f754e5a, "memset" },
	{ 0xb9757c96, "cdev_add" },
	{ 0xfad496fc, "cdev_init" },
	{ 0xdb7305a1, "__stack_chk_fail" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xd1f18866, "cdev_del" },
	{ 0x7c32d0f0, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:ov7670");
