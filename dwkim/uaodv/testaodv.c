#include "contiki.h"
#include "dev/leds.h"
#include "net/rime.h"

#include "net/uaodv.h"
#include "net/uaodv-rt.h"

static struct uip_udp_conn *out_conn;
static struct uip_udp_conn *in_conn;

#define COOJA_PORT 1234

#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD( hello_world_process, ev, data) {
	static uip_ipaddr_t addr;

	PROCESS_BEGIN()
	;

	//for timer  // kdw
	static struct etimer et;
	etimer_set(&et, CLOCK_SECOND * 4 + random_rand() % (CLOCK_SECOND * 4));

	printf("uIP uAODV test process started\n");

	uip_ipaddr(&addr, 0, 0, 0, 0);
	in_conn = udp_new(&addr, UIP_HTONS(0), NULL);
	uip_udp_bind(in_conn, UIP_HTONS(COOJA_PORT));

	uip_ipaddr(&addr, 10, 10, 10, 4);
	out_conn = udp_new(&addr, UIP_HTONS(COOJA_PORT), NULL);

	//button_sensor.configure(SENSORS_ACTIVE, 1);

	while (1) {
		PROCESS_WAIT_EVENT();

//	    if(ev == sensors_event && data == &button_sensor) {
		if (etimer_expired(&et)) {

			struct uaodv_rt_entry *route;

			uip_ipaddr(&addr, 10, 10, 10, 4);
			route = uaodv_rt_lookup_any(&addr);
			if (route == NULL || route->is_bad) {
				printf("%d.%d.%d.%d: lookup %d.%d.%d.%d\n",
						uip_ipaddr_to_quad(&uip_hostaddr), uip_ipaddr_to_quad(&addr));
						uaodv_request_route_to(&addr);
					} else {
						printf("%d.%d.%d.%d: send to %d.%d.%d.%d\n", uip_ipaddr_to_quad(&uip_hostaddr), uip_ipaddr_to_quad(&addr));
						tcpip_poll_udp(out_conn);
						PROCESS_WAIT_UNTIL(ev == tcpip_event && uip_poll());
						uip_send("cooyah COOJA", 12);
					}
				}

		if (ev == tcpip_event && uip_newdata()) {
			((char*) uip_appdata)[uip_datalen()] = 0;
			printf("data received from %d.%d.%d.%d: %s\n",
					uip_ipaddr_to_quad(&((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])->srcipaddr) ,
					(char *)uip_appdata);
			leds_toggle(LEDS_ALL);
		}
	}

PROCESS_END();
}
/*---------------------------------------------------------------------------*/
