#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(demo_source, "Demo_source");
AUTOSTART_PROCESSES(&demo_source);
/*---------------------------------------------------------------------------*/
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc, sent_uc };
static struct unicast_conn unicast;
/*---------------------------------------------------------------------------*/PROCESS_THREAD(demo_source, ev, data) {
	rimeaddr_t addr;
	char* str;
	uint8_t test[10]={1,2,3,4,5,6,7,8,9,0};
	static struct etimer et;

	PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN()
	;


	// unicast init //
	unicast_open(&unicast, 146, &unicast_callbacks);

	while (1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		leds_on(1);

		str="Start";
		//packetbuf_copyfrom(str, 6);		//forward "START" message
		packetbuf_copyfrom(test, 10);		//forward "START" message

		addr.u8[0] = 198;
		addr.u8[1] = 64; 	//transmit to Source1

	    if(!rimeaddr_cmp(&addr, &rimeaddr_node_addr)) {
	    	unicast_send(&unicast, &addr);
	      printf("send");
	    }
	}
PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
char* temp_string = (char *) packetbuf_dataptr();
//uint8_t input = packetbuf_dataptr()
printf("unicast message received from %d.%d\t '%c'\n", from->u8[0], from->u8[1],
		temp_string[0]);
printf("%d\t%d\t%d\t%d\t%d\n", (uint8_t)(temp_string[1]),(uint8_t)(temp_string[2]),(uint8_t)(temp_string[3]),(uint8_t)(temp_string[4]),(uint8_t)(temp_string[5]));
}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
printf("sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0],
		rimeaddr_node_addr.u8[1],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}


