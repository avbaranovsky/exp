#include <string.h>
#include <stdio.h>

int f1() {
	void *buf = 0;
	memset(buf, 0, 1024);
	return 0;
}

int f2() {
	f1();
}

void main() {
	f2();
}

