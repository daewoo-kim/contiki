/*
 * Copyright (c) 2006, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * $Id: hello-world.c,v 1.1 2006/10/02 21:46:46 adamdunkels Exp $
 */

/**
 * \file
 *         A very simple Contiki application showing how Contiki programs look
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "dev/leds.h"
//#include "../config.h"
//#include "usb_drv.h"
//#include "usb_descriptors.h"
#include "uart_usb_lib.h"
//#include "cdc_task.h"

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
//PROCESS(blink_process, "LED blink process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  PROCESS_BEGIN();
//* init
  leds_init();


  printf("Start Main Process\n");
  leds_on(4);

  static struct etimer timer;
  static uint8_t leds_state = 0;



  while (1)
  {
     // we set the timer from here every time
     etimer_set(&timer, CLOCK_CONF_SECOND / 4);

     // and wait until the vent we receive is the one we're waiting for
     PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);

     // update the LEDs
     leds_off(0xFF);
     leds_on(leds_state%2);
     leds_state += 1;


  }



  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
///* Implementation of the second process */
//PROCESS_THREAD(blink_process, ev, data)
//{
//
//    PROCESS_BEGIN();
//    	printf("I'm here\n");
//    	leds_on(4);
//    PROCESS_END();
//}
///*---------------------------------------------------------------------------*/
