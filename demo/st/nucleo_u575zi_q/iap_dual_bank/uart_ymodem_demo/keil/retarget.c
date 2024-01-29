#include <rt_sys.h>
#include <rt_misc.h>
#include <time.h>

#if (__ARMCC_VERSION >= 6000000)
    __asm(".global __use_no_semihosting");
#else
    #pragma import(__use_no_semihosting)
#endif

const char __stdin_name[] =  ":tt";
const char __stdout_name[] =  ":tt";
const char __stderr_name[] =  ":tt";

FILEHANDLE _sys_open(const char *name, int openmode)
{
   return 1;
}

int _sys_close(FILEHANDLE fh)
{
   return 0;
}

char *_sys_command_string(char *cmd, int len)
{
   return NULL;
}

int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
{
   return 0;
}

int _sys_read(FILEHANDLE fh, unsigned char *buf, unsigned len, int mode)
{
   return -1;
}

void _ttywrch(int ch)
{
}

int _sys_istty(FILEHANDLE fh)
{
   return 0;
}

int _sys_seek(FILEHANDLE fh, long pos)
{
   return -1;
}

long _sys_flen(FILEHANDLE fh)
{
   return -1;
}

void _sys_exit(int return_code)
{
   while(1)
   {
   }
}

clock_t clock(void)
{
   return 0;
}

void _clock_init(void)
{
}

time_t time(time_t *timer)
{
   return 0;
}

int system(const char *string)
{
   return 0;
}

char *getenv(const char *name)
{
   return NULL;
}

void _getenv_init(void)
{
}
