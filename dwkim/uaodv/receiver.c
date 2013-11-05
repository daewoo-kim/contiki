#include "contiki.h"
#include "dev/leds.h"

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

#define MAX_PAYLOAD_LEN		40
#define SEND_INTERVAL			3 * CLOCK_SECOND
#define PORT_DW				1234
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(uip_receiver, "uip_sender");
AUTOSTART_PROCESSES(&uip_receiver);
/*---------------------------------------------------------------------------*/

static struct uip_udp_conn *in_conn;

static char buf[MAX_PAYLOAD_LEN];
static void reciever(void) {
	static int seq_id;
	char buf[MAX_PAYLOAD_LEN];

	if (uip_newdata()) {
		((char *) uip_appdata)[uip_datalen()] = 0;
		PRINTF("Received: '%s' from ", (char *)uip_appdata);PRINT6ADDR(&UIP_IP_BUF->srcipaddr);PRINTF("\n");

		uip_ipaddr_copy(&in_conn->ripaddr, &UIP_IP_BUF->srcipaddr);
		PRINTF("Responding with message: ");
		sprintf(buf, "Hello from the server! (%d)", ++seq_id);
		PRINTF("%s\n", buf);

		uip_udp_packet_send(in_conn, buf, strlen(buf));
		/* Restore server connection to allow data from any node */
		memset(&in_conn->ripaddr, 0, sizeof(in_conn->ripaddr));
	}
}

/*---------------------------------------------------------------------------*/
//
PROCESS_THREAD(uip_receiver, ev, data) {

	static uip_ipaddr_t addr;

	PROCESS_BEGIN();

//		uip_ipaddr(&addr, 0, 0, 0, 0);
//		in_conn = udp_new(&addr, UIP_HTONS(0), NULL);
//		uip_udp_bind(in_conn, UIP_HTONS(PORT_DW));


		printf("uIP RECEIVE test process started\n");

		while (1) {
			PROCESS_YIELD()			;
			if (ev == tcpip_event) {
				reciever();
			}
		}

		PROCESS_END();
}
/*---------------------------------------------------------------------------*/
