#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "demo.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(demo_source, "Demo_source");
AUTOSTART_PROCESSES(&demo_source);
/*---------------------------------------------------------------------------*/
static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from);
static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from);
static const struct unicast_callbacks unicast_callbacks = { recv_uc, sent_uc };
static struct unicast_conn unicast;
/*--------------- for UART ---------------------------------------------------*/
int kimdaewoo=1;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo_source, ev, data) {
	static struct etimer et;
	rimeaddr_t addr;

	PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN();

	// unicast //
	unicast_open(&unicast, 146, &unicast_callbacks);

	// UART //
//	slip_arch_init(115200);
	static int flag =1;

	while (1) {
		etimer_set(&et, CLOCK_SECOND);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


		if(flag==1)
		{
			// set address //
			addr.u8[0] = 1;
			addr.u8[1] = 0;
			char* testString = "1111111111222222222233333333334444444444555555555566666666667777777777888888888899999999990000000000111111111112";

			packetbuf_copyfrom(testString, 110);

			if (!rimeaddr_cmp(&addr, &rimeaddr_node_addr)) {
				unicast_send(&unicast, &addr);
				flag = 0;
			}
		}

	}
PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	printf("unicast message received from %d.%d\n", from->u8[0], from->u8[1]);
}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	printf("sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0],
		rimeaddr_node_addr.u8[1],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}
int UART_out(char out){
	slip_arch_writeb(out);
	return 1;
}

