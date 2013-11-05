
#include "contiki.h"
#include "node-id.h"
#include "dev/leds.h"
#include "net/rime.h"



#include <stdio.h> /* For printf() */
/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD( hello_world_process, ev, data) {
	uint16_t nodeId = 6;
	rimeaddr_t myaddr;
	myaddr.u8[0]=0;
	myaddr.u8[0]=1;


	PROCESS_BEGIN();


	printf("Burning node id %d\n", nodeId);
	node_id_burn(nodeId);
	leds_on(7);
	node_id_restore();
	printf("Restored node id %d\n", node_id);
	leds_on(0);


	//rimeaddr_set_node_addr(&myaddr);

	printf("%d.%d: my address\n",
				rimeaddr_node_addr.u8[0], rimeaddr_node_addr.u8[1]);



	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
