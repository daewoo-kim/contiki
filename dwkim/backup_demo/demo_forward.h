#ifndef _DEMO_FORWARD
#define _DEMO_FORWARD

#include "demoheader.h"

#define DEBUG 1 //  원래는 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
#define PRINTF(...)
#endif /* DEBUG */

#endif /* _DEMO */
