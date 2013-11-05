#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "demo_sink.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(demo_sink, "Demo_sink");
AUTOSTART_PROCESSES(&demo_sink);
/*---------------------------------------------------------------------------*/
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc, sent_uc };
static struct unicast_conn unicast;
char* sensingdata;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo_sink, ev, data) {
	static struct etimer et;

	PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN();

	// unicast //
	unicast_open(&unicast, 146, &unicast_callbacks);

	watchdog_stop();

	while (1) {
		etimer_set(&et, CLOCK_SECOND /2 );
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
		leds_on(1);

	}

PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	sensingdata = (char *) packetbuf_dataptr();
	PRINTF("unicast message received from %d.%d\t '%c'\n", from->u8[0], from->u8[1],sensingdata[0] );
	PRINTF("Data: %d %d %d %d %d\n",sensingdata[1],sensingdata[2],sensingdata[3],sensingdata[4],sensingdata[5]);

}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	printf("sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0],
		rimeaddr_node_addr.u8[1],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}
