#ifndef _DEMO_INIT
#define _DEMO_INIT

#define DEBUG 1 //  원래는 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
#define PRINTF(...)
#endif /* DEBUG */

#define INIT_WAIT 		0
#define INIT_SEND 		1

extern int INIT_STATE;

#endif /* _DEMO */
