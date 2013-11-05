#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "demo_forward.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(demo_source, "Demo_source");
AUTOSTART_PROCESSES(&demo_source);
/*---------------------------------------------------------------------------*/
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc, sent_uc };
static struct unicast_conn unicast;
char* sensingdata;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo_source, ev, data) {
	static struct etimer et;

	PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN();

	// unicast init //
	unicast_open(&unicast, 146, &unicast_callbacks);


	while (1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		leds_on(1);


	}
PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	char* temp_string = (char *) packetbuf_dataptr();
	PRINTF("unicast message received from %d.%d\t '%c'\n", from->u8[0], from->u8[1],temp_string[0] );
	if(temp_string[0]=='S'){
		forward_init();
	}
	else if(temp_string[0]=='A'||temp_string[0]=='B'){
		sensingdata=temp_string;
		forward_data();
	}
}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	PRINTF("sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0],
		rimeaddr_node_addr.u8[1],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}

void forward_init(){
	rimeaddr_t addr;
	packetbuf_copyfrom("START", 6);		//forward "START" message
	addr.u8[0] = 4;	addr.u8[1] = 0; 	//transmit to F2
	unicast_send(&unicast, &addr);
	leds_on(7);
}

void forward_data(){
	PRINTF("Data: %d %d %d %d %d\n",sensingdata[1],sensingdata[2],sensingdata[3],sensingdata[4],sensingdata[5]);
	rimeaddr_t addr;
	leds_on(7);
	packetbuf_copyfrom(sensingdata, UART_Full+1);		//forward "START" message
	addr.u8[0] = 1;	addr.u8[1] = 0; 	//transmit to Source1
	unicast_send(&unicast, &addr);

	}

