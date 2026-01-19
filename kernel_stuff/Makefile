obj-m += test_mod.o

KDIR=/usr/src/linux-headers-6.12.34+rpt-rpi-2712

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules
clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
	rm -f *.o