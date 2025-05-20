#ifndef _FNS_H_
#define _FNS_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#define nelem(x) (sizeof(x)/sizeof(x[0]))
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

void uartwrite(const char *buf, size_t len);

uint32_t rdcntfreq(void);
uint32_t rdcntlpct(void);

uint32_t rdcpsr(void);
void wrcpsr(uint32_t val);

uint32_t rdvbar(void);
void wrvbar(void *vbar);

void gicdump(void);
void gicinit(void);

void gpiodump(void);
void gpioinit(void);

#endif
