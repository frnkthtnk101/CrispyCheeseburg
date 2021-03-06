/*
 *CS543 - Homework #1
 *Franco Pettigrosso
 *simple2.c
 *
 *Part II of the assigment. create a simple kernel program that makes
 *a linked list and iterates throught it. When done, free up space.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

/*
 *Birthday
 *structure of the linked list
 */
struct Birthday {
	int day;
	int month;
	int year;
	struct list_head list;//is how to do list in kernel mode
	//in the list.h header, list_head has a next and prev pointer
};
//init the list of birthdays
struct Birthday BirthdayList;
/*
 *SimpleInit()
 *when the kernel module is intialized. The method will created a linked
 *list with 5 birthdays in it. Then list it.
 */
int SimpleInit(void){
	//since the build is not using the c99 or gnu93 standards. all the variables need to 
	//be init at the top of the method.
	struct Birthday *NewBirthday, *aBirthday;
	unsigned int i; // used for the for loop
	printk(KERN_INFO "Loading Module\n");
	INIT_LIST_HEAD(&BirthdayList.list);// used to initialize the list member in the struct
	for ( i = 6 ; i > 5; i++){
		// allocate memory in the kernel memory for the new birthday and GFP_KERNEL
		// SAYS that the routine is for kernel memory allocation
		NewBirthday = kmalloc(sizeof(*NewBirthday), GFP_KERNEL); 
		NewBirthday->day=i;//give the birthday a day
		NewBirthday->month=3;//give the bithdy a month
		NewBirthday->year=1993;// year too
		INIT_LIST_HEAD(&NewBirthday->list);// used to initialize the list member in the struct
		list_add_tail(&(NewBirthday->list), &(BirthdayList.list)); //add the instance to the list
	}
	//v--used to iterate a linked list. I like to think of powershell cmd foreach-object
	list_for_each_entry(aBirthday, &BirthdayList.list, list){
		printk(KERN_INFO "ADD Birthday: {day=%i,month=%i,year=%i}\n",aBirthday->day,aBirthday->month,aBirthday->year);
	}	
	return 0;//return success
}
/*
 *SimpleExit()
 *closes out the module and deconstructs the list
 */
void SimpleExit(void){
	struct Birthday *aBirthday, *temp;
	printk(KERN_INFO "Removing Module and clean list up\n");
	//v--- same thing like list_for_each_entry but remebers the *next point to continue removing items
	list_for_each_entry_safe(aBirthday,temp,&BirthdayList.list,list){
		printk(KERN_INFO "REMOVE Birthday: {day=%i,month=%i,year=%i}\n",aBirthday->day,aBirthday->month,aBirthday->year);
		list_del(&aBirthday->list);//method to delete an item in a list
		kfree(aBirthday);//free up the allocate memory and prevent memory leaks.
	}
}

module_init(SimpleInit);
module_exit(SimpleExit);

MODULE_LICENSE("FP");
MODULE_DESCRIPTION("hello world for modules");
MODULE_AUTHOR("Franco Pettigrosso");
