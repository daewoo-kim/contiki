/**
 * \file
 *         A very simple Contiki application showing to independent processes running
 * \author
 *         Clément Burin des Roziers <clement.burin-des-roziers@inria.fr>
 */

#include "contiki.h"
#include "dev/leds.h"
#include "net/rime.h" 

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
/* We declare the two processes */
PROCESS(hello_world_process, "Hello world process");
//PROCESS(blink_process, "LED blink process");

/* We require the processes to be started automatically */
AUTOSTART_PROCESSES(&hello_world_process);//, &blink_process);
/*---------------------------------------------------------------------------*/


//--// 선언부  //
static struct etimer et;
static struct broadcast_conn bc;
static const struct broadcast_callbacks broadcast_callbacks = {recv_bc};
static struct unicast_conn uc;
static const struct unicast_callbacks unicast_callbacks = {recv_uc};

static void
recv_bc(struct broadcast_conn *c, rimeaddr_t *from)
{
  printf("broadcast from %02x.%02x len = %d buf = %s\n",
		  from->u8[0],
		  from->u8[1],
		  packetbuf_datalen(),
		  (char *)packetbuf_dataptr());
}

static void
recv_uc(struct unicast_conn *c, rimeaddr_t *from)
{
  printf("unicast from %02x.%02x len = %d buf = %s\n",
		  from->u8[0],
		  from->u8[1],
		  packetbuf_datalen(),
		  (char *)packetbuf_dataptr());
}

/* Implementation of the first process */
PROCESS_THREAD(hello_world_process, ev, data)
{
    // variables are declared static to ensure their values are kept
    // between kernel calls.
    //static struct etimer timer;
    static int count = 0;
    
    // any process mustt start with this.
    PROCESS_BEGIN();

    printf("\nStarting CC2430 RF test suite...\n");

    broadcast_open(&bc, 128, &broadcast_callbacks);
    unicast_open(&uc, 128, &unicast_callbacks);
    // set the etimer module to generate an event in one second.
    //etimer_set(&timer, CLOCK_CONF_SECOND);




    while (1)
    {
        // wait here for an event to happen
    	etimer_set(&et, CLOCK_SECOND);
    	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    	etimer_reset(&et);
	
	

	leds_toggle(1);

/*
//PROCESS_WAIT_EVENT();

        // if the event is the timer event as expected...
        if(ev == PROCESS_EVENT_TIMER)
        {


            // do the process work
            printf("Hello, world #%i\n", count);
            count ++;
            // reset the timer so it will generate an other event
            // the exact same time after it expired (periodicity guaranteed)
            etimer_reset(&timer);

	    //for led
	    leds_on(count);
	    
            
        }
        
        // and loop*/

	


    }
    // any process must end with this, even if it is never reached.
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/* Implementation of the second process */
/*PROCESS_THREAD(blink_process, ev, data)
{
    static struct etimer timer;
    static uint8_t leds_state = 0;
    PROCESS_BEGIN();
    
    while (1)
    {
        // we set the timer from here every time
        etimer_set(&timer, CLOCK_CONF_SECOND / 4);
        
        // and wait until the vent we receive is the one we're waiting for
        PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
        
        // update the LEDs
        leds_off(0xFF);
        leds_on(leds_state);
        leds_state += 1;
    }
    PROCESS_END();
}*/
/*---------------------------------------------------------------------------*/
