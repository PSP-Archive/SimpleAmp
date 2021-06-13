#include <pspkernel.h>
#include <pspdebug.h>
#include <pspkdebug.h>
#include <pspsdk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usbhostfs.h"

int build_args(char *args, const char *execfile, int argc, char **argv);
int load_start_module(const char *name, int argc, char **argv);
int start_usbhost(const char *bootpath);
int stop_usbhost(void);

