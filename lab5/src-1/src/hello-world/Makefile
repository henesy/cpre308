obj-m += hello_world.o

LINUX_SRC = /tmp/seh/linux-4.10.1/

all:
	make -C $(LINUX_SRC) M=$(PWD) modules

clean:
	make -C $(LINUX_SRC) M=$(PWD) clean

