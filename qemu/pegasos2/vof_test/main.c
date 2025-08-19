/*

Calling into Open Firmware (VOF) services:

1. Place the prom_args structure physical address into r3
2. Jump to vof_entry (vof_entry is given by the firmware on startup in r5)

The code at vof_entry does the following:
1. Store the prom_args from r3 to r4
2. Set r3 to 0xff05 to specify it wants to do a VOF service when initiating a hypercall
3. Do the hypercall

To obtain the code for calling RTAS:
1. Call vof open /rtas
2. Call vof call-method "instantiate-rtas" with the physical address of a buffer that is executable
3. The buffer will now be filled with code that can be jumped to in order to execute an RTAS call

*/

void reboot(void);

typedef unsigned char u8;
typedef unsigned int u32;

typedef struct {
	u32 service;
	u32 nargs;
	u32 nret;
	u32 args[10];
} prom_args;

typedef struct {
	int token;
	int nargs;
	int nret;
	int args[16];
} rtas_args;

typedef int (*prom_func)(void *);
typedef int (*rtas_func)(void *);
typedef void (*void_func)(void);

prom_func vof_handler;
rtas_func rtas_handler;
u8 rtas_code[32];
int numreboots;

void
memset(void *s, int c, int n)
{
	char *p;
	int i;

	p = s;
	for (i = 0; i < n; i++)
		p[i] = c;
}

int
strlen(const char *str)
{
	int len;

	len = 0;
	while (*str++)
		len++;
	return len;
}

int
vof_open(const char *name)
{
	prom_args args;

	memset(&args, 0, sizeof(args));
	args.service = (u32) "open";
	args.nargs = 1;
	args.nret = 1;
	args.args[0] = (u32)name;
	vof_handler(&args);
	return args.args[1];
}

void
vof_close(u32 handle)
{
	prom_args args;

	memset(&args, 0, sizeof(args));
	args.service = (u32) "close";
	args.nargs = 1;
	args.nret = 0;
	args.args[0] = handle;
	vof_handler(&args);
}

void
vof_write(u32 handle, const void *buf, int len)
{
	prom_args args;

	memset(&args, 0, sizeof(args));
	args.service = (u32) "write";
	args.nargs = 3;
	args.nret = 1;
	args.args[0] = handle;
	args.args[1] = (u32)buf;
	args.args[2] = len;
	vof_handler(&args);
}

u32
vof_milliseconds(void)
{
	prom_args args;

	memset(&args, 0, sizeof(args));
	args.service = (u32) "milliseconds";
	args.nargs = 0;
	args.nret = 1;
	vof_handler(&args);
	return args.args[0];
}

void
vof_exit(void)
{
	prom_args args;

	memset(&args, 0, sizeof(args));
	args.service = (u32) "exit";
	args.nargs = 0;
	args.nret = 0;
	vof_handler(&args);
}

u32
vof_get_rtas_entry(void)
{
	u32 handle;
	prom_args args;

	handle = vof_open("/rtas");
	memset(&args, 0, sizeof(args));
	args.service = (u32) "call-method";
	args.nargs = 3;
	args.nret = 1;
	args.args[0] = (u32) "instantiate-rtas";
	args.args[1] = handle;
	args.args[2] = (u32)rtas_code;
	vof_handler(&args);
	vof_close(handle);

	return (u32)rtas_code;
}

void
print_number(u32 handle, u32 value)
{
	char buf[16], out[16];
	int i, len;

	len = 0;
	do {
		buf[len++] = '0' + (value % 10);
		value /= 10;
	} while (value > 0);

	for (i = 0; i < len; i++)
		out[i] = buf[len - i - 1];

	vof_write(handle, out, len);
}

void
rtas_puts(const char *str)
{
	rtas_args args;
	int i;

	memset(&args, 0, sizeof(args));
	args.token = 10;
	args.nargs = 1;
	args.nret = 1;
	for (i = 0; str[i]; i++) {
		args.args[0] = str[i];
		rtas_handler(&args);
	}
}

void
rtas_shutdown(void)
{
	rtas_args args;

	memset(&args, 0, sizeof(args));
	args.token = 17;
	args.nargs = 2;
	args.nret = 1;
	rtas_handler(&args);
}

void
vof_test(u32, u32, u32 entry, u32, u32)
{
	u32 handle;
	int i;

	vof_handler = (prom_func)entry;
	rtas_handler = (rtas_func)vof_get_rtas_entry();
	rtas_puts("Hello from RTAS\n");

	handle = vof_open("/failsafe");
	for (i = 0; i < 16; i++) {
		vof_write(handle, "hello world!", 12);
		print_number(handle, vof_milliseconds());
	}

	vof_close(handle);
	if (numreboots < 10000) {
		numreboots++;
		reboot();
	}
	rtas_shutdown();
	vof_exit();

	for (;;)
		;
}
