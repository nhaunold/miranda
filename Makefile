all:
	arm-elf-gcc -s -Ttext=0x80000400 -nostdlib -o code.o code.S
	arm-elf-objcopy -O binary code.o code.bin
	rm code.o
	/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/arm-apple-darwin10-llvm-gcc-4.2 -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.0.sdk -framework IOKit -framework CoreFoundation -I. exp.c -o exp
	ldid -S exp


install:	all
	scp code.bin root@192.168.1.146:
	scp exp root@192.168.1.146:

install-ipad:	all
	scp code.bin root@192.168.1.108:
	scp exp root@192.168.1.108:

clean:
	rm -f exp code.bin
