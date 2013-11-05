
#include "contiki.h"
#include "dev/leds.h"

#include "node-id.h"		//kdw
#include <stdio.h>
#include "multihop.c"

#define NEIGHBOR_TIMEOUT 60 * CLOCK_SECOND
/*---------------------------------------------------------------------------*/
PROCESS(example_multihop_process, "multihop example");
AUTOSTART_PROCESSES(&example_multihop_process);

/*---------------------------------------------------------------------------*/PROCESS_THREAD(
		example_multihop_process, ev, data) {
	static struct etimer et;


	/*  For nodeId allocation */
	uint16_t nodeId = 3;

	PROCESS_BEGIN();

	/* Wait 2000ms. */
	etimer_set(&et, CLOCK_SECOND * 2);

	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

	printf("Burning node id %d\n", nodeId);
	node_id_burn(nodeId);
	leds_on(LEDS_BLUE);
	node_id_restore();
	printf("Restored node id %d\n", node_id);


	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
