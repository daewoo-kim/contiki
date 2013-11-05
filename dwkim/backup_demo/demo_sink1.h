#ifndef _DEMO_SINK1
#define _DEMO_SINK1

#include "demoheader.h"

#define DEBUG 0 //  원래는 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
#define PRINTF(...)
#endif /* DEBUG */


#endif /* _DEMO */
