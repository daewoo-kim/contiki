#include "contiki.h"
#include "net/rime.h"
#include "dev/leds.h"
#include "demo1.h"

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
int DEMO_STATE = DEMO_LISTEN;
int DEMO_InputCount = 0;
uint8_t* data_sensing;	//for storing data
int count=0;
int flag_c=0;

/*---------------------------------------------------------------------------*/
	PROCESS_THREAD(demo_source, ev, data) {
	static struct etimer et;
	rimeaddr_t addr;


	//PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN()
		;

		// unicast init //
		unicast_open(&unicast, 146, &unicast_callbacks);

		// UART init //
		slip_arch_init(115200);

		//  DATA Storing //
		data_sensing = (uint8_t *) malloc(UART_Full + 1 * sizeof(uint8_t));	//size+1 은 첫번째에 indicator 넣기 위함!
		data_sensing[0] = 'B';	//sensor 식별자 sensor A or sensor B
		//	data_sensing[0]='B';	//sensor 식별자 sensor A or sensor B
		leds_off(7);
		while (1) {


			etimer_set(&et, CLOCK_SECOND / 10);
			PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


			if(flag_c==1){
				count++;
				leds_on(4);
			}
			if(count==20){
				leds_off(4);
				count=0;
				flag_c=0;
			}


			/* DEMO_LISTEN :
			 * Listen from sensor by serial port 			*/
			if (DEMO_STATE == DEMO_LISTEN) {
				PRINTF("DEMO_STATE: %d. DEMO_LISTEN\n",DEMO_STATE);
				leds_off(3);	leds_on(1);
			}
			/* DEMO_SEND :
			 *  count max -> send   */
			else if (DEMO_STATE == DEMO_SEND) {
				PRINTF("DEMO_STATE: %d. DEMO_SEND\n",DEMO_STATE);
				leds_off(3);	leds_on(2);

				//헤더 붙이고, 데이터 패킷화 하여 전송하기
				packetbuf_copyfrom(data_sensing, UART_Full + 1);
				addr.u8[0] = 4;		addr.u8[1] = 0;	//send to F2
//				addr.u8[0] = 2;		addr.u8[1] = 0;	//for test

				unicast_send(&unicast, &addr);

				DEMO_STATE = DEMO_LISTEN;
			} else {
				PRINTF("NONE STATE \t%d\n",DEMO_STATE);

			}
		}
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	char* temp_string = (char *) packetbuf_dataptr();
	PRINTF("unicast message received from %d.%d\t '%s'\n", from->u8[0], from->u8[1],temp_string );
	flag_c=1;
}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	PRINTF("sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0],
		rimeaddr_node_addr.u8[1],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}
int UART_out(char out) {
slip_arch_writeb(out);
return 1;
}

