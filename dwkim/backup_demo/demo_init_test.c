#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "demo_init.h"

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
	static struct etimer et;
	rimeaddr_t addr;

	PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN()
	;

	// unicast init //
	unicast_open(&unicast, 146, &unicast_callbacks);

	etimer_set(&et, CLOCK_SECOND / 10);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	leds_on(7);
	packetbuf_copyfrom("START", 6);		//Send "START" message
	addr.u8[0] = 3;
	addr.u8[1] = 0; 	//transmit to F1
	unicast_send(&unicast, &addr);

PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
char* temp_string = (char *) packetbuf_dataptr();
PRINTF(
		"unicast message received from %d.%d\t '%s'\n", from->u8[0], from->u8[1], temp_string);
}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
PRINTF(
		"sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1], packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0], packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}

