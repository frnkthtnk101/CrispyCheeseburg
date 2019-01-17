/*
 *simle1.c
 *the hello world for modules
 *Franco Pettigrosso
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/*
 *SimpleInit()
 *when the kernel module is initialized it will print to the kernel log
 */
int SimpleInit(void){
	printk(KERN_INFO "HELLO WORLD!\n");
	return 0;
}
/*
 *SimpleExit()
 *when the kernel module is deconstructed it will print to the kernel log
 */
void SimpleExit(void){
	printk(KERN_INFO "Goodbye World!\n");
}

//methods for registering module entry point and exit points
module_init(SimpleInit);
module_exit(SimpleExit);
//stuff about the description
MODULE_LICENSE("FP");
MODULE_DESCRIPTION("hello world for modules");
MODULE_AUTHOR("Franco Pettigrosso");
