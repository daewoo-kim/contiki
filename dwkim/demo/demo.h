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

#define RAIDO_LISTEN 		0
#define UART_START 		1
#define UART_SEND 		2
#define UART_LISTEN 		3
#define UART_READY		4
#define RADIO_SENT 		5

//#define UART_Full			5
//#define END_SENSING		5


extern int CURRENT_STATE;
extern int UART_End;
extern int UART_NumInput;
extern int UART_Success;
extern int UART_End;


extern uint8_t* data_sensing;	//for storing data

int UART_out(char out);

#endif /* _DEMO */
