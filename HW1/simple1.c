#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

struct Birthday {
	int day;
	int month;
	int year;
	struct list_head list;
};

struct Birthday BirthdayList;

int SimpleInit(void){
	struct Birthday *NewBirthday, *aBirthday;
	unsigned int i;
	printk(KERN_INFO "Loading Module\n");
	INIT_LIST_HEAD(&BirthdayList.list);
	for ( i = 0 ; i < 5; i++){
		NewBirthday = kmalloc(sizeof(*NewBirthday), GFP_KERNEL);
		NewBirthday->day=i;
		NewBirthday->month=3;
		NewBirthday->year=1993;
		INIT_LIST_HEAD(&NewBirthday->list);
		list_add_tail(&(NewBirthday->list), &(BirthdayList.list));
	}
	list_for_each_entry(aBirthday, &BirthdayList.list, list){
		printk(KERN_INFO "ADD Birthday: {day=%i,month=%i,year=%i}\n",aBirthday->day,aBirthday->month,aBirthday->year);
	}	
	return 0;
}

void SimpleExit(void){
	struct Birthday *aBirthday, *temp;
	printk(KERN_INFO "Removing Module and clean list up\n");
	list_for_each_entry_safe(aBirthday,temp,&BirthdayList.list,list){
		printk(KERN_INFO "REMOVE Birthday: {day=%i,month=%i,year=%i}\n",aBirthday->day,aBirthday->month,aBirthday->year);
		list_del(&aBirthday->list);
		kfree(aBirthday);
	}
}

module_init(SimpleInit);
module_exit(SimpleExit);

MODULE_LICENSE("FP");
MODULE_DESCRIPTION("hello world for modules");
MODULE_AUTHOR("Franco Pettigrosso");
