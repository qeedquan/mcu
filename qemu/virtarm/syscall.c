#include "dat.h"
#include "fns.h"

void
uartwrite(const char *buf, size_t len)
{
	enum {
		DR = 0x0 / 4,
		FR = 0x18 / 4,

		FR_TXFF = 0x20,
		FR_RXFE = 0x10,
		FR_BUSY = 0x08,
	};

	volatile uint32_t *regs;
	size_t i;

	regs = (volatile uint32_t *)0x9000000;
	for (i = 0; i < len; i++) {
		while (regs[FR] & FR_TXFF)
			;
		regs[DR] = buf[i];
	}
}

void *
_sbrk(int incr)
{
	static unsigned char *heap = (unsigned char *)&__heap_start__;
	void *ptr;

	ptr = heap;
	if ((unsigned char *)&__heap_end__ - heap < incr)
		return NULL;

	heap += incr;
	return ptr;
}

void
_exit(int status)
{
	for (;;)
		;
}

int
_open(const char *name, int flags, mode_t mode)
{
	return -1;
}

int
_lseek(int fd, off_t off, int whence)
{
	return -1;
}

ssize_t
_write(int fd, void *buf, size_t len)
{
	uartwrite(buf, len);
	return len;
}

ssize_t
_read(int fd, void *buf, size_t len)
{
	return -1;
}

int
_fstat(int fd, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int
_close(int fd)
{
	return -1;
}

int
_kill(int pid, int sig)
{
	return -1;
}

pid_t
_getpid(void)
{
	return 1;
}

int
_isatty(int fd)
{
	return 1;
}

int
_gettimeofday(struct timeval *tv, struct timezone *tz)
{
	tv->tv_sec = rdcntlpct() / rdcntfreq();
	return 0;
}

unsigned
sleep(unsigned sec)
{
	time_t t0, t1, dt;

	t0 = time(NULL);
	while (sec > 0) {
		t1 = time(NULL);
		if (t1 <= t0)
			continue;

		dt = t1 - t0;
		if (dt >= sec)
			dt = sec;
		sec -= dt;
		t1 = t0;
	}
	return 0;
}
