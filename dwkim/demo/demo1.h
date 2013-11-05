#ifndef _DEMO
#define _DEMO

#include "demoheader.h"

#define DEBUG 0 //  debug-mode:1  non-debug-mode:0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else /* DEBUG */
#define PRINTF(...)
#endif /* DEBUG */

#define DEMO_LISTEN		0
#define DEMO_SEND 		1


extern int DEMO_STATE;
extern int DEMO_InputCount;

extern uint8_t* data_sensing;	//for storing data

int UART_out(char out);

#endif /* _DEMO */
