obj-m += simplecrash.o
obj-m += simple1.o
obj-m += simple2.o
obj-m += simple3.o
obj-m += simple4.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 
