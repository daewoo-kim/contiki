#include "contiki.h"
#include "dev/leds.h"

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

#define MAX_PAYLOAD_LEN		40
#define SEND_INTERVAL			3 * CLOCK_SECOND
#define PORT_DW				1234

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(uip_sender, "uip_sender");
AUTOSTART_PROCESSES(&uip_sender);
/*---------------------------------------------------------------------------*/
static struct uip_udp_conn *out_conn;
static struct uip_udp_conn *in_conn;

static char buf[MAX_PAYLOAD_LEN];
static void sender(void) {
	static int seq_id;

	printf("Client sending to: ");
	PRINT6ADDR(&out_conn->ripaddr);
//	printf("%d.%d.%d.%d: send to %d.%d.%d.%d\n", uip_ipaddr_to_quad(&out_conn), uip_ipaddr_to_quad(&out_conn->ripaddr));

	sprintf(buf, "Hello %d from the client", ++seq_id);
	printf(" (msg: %s)\n", buf);
	uip_udp_packet_send(out_conn, buf, strlen(buf));
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(uip_sender, ev, data) {

	static uip_ipaddr_t addr;

	PROCESS_BEGIN();


//	uip_ipaddr(&addr, 0,0,0,0);
//	in_conn = udp_new(&addr, UIP_HTONS(0), NULL);
//	uip_udp_bind(in_conn, UIP_HTONS(PORT_DW));

	uip_ipaddr(&addr, 10,10,10,4);
	out_conn = udp_new(&addr, UIP_HTONS(PORT_DW), NULL);

		static struct etimer et;
		printf("uIP SEND test process started\n");

		etimer_set(&et, SEND_INTERVAL);
		while (1) {
			PROCESS_YIELD();
			if (etimer_expired(&et)) {
//				printf("time expired\n");
				sender();
				etimer_restart(&et);
			}
		}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
