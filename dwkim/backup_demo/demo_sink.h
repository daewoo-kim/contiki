#ifndef _DEMO_SINK
#define _DEMO_SINK

#include "demoheader.h"

#define DEBUG 0 //  원래는 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
#define PRINTF(...)
#endif /* DEBUG */


#define SINK_LISTEN 		0
#define SINK_INIT 		1

extern int SINK_STATE;

#endif /* _DEMO */
