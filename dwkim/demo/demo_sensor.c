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
int CURRENT_STATE=RAIDO_LISTEN;
char RADIO_Input;
int RADIO_NumSend=0;
int UART_End=0;
int UART_NumInput=0;
int UART_Success=1;
uint8_t* data_sensing;	//for storing data

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(demo_source, ev, data) {
	static struct etimer et;
	rimeaddr_t addr;

	//PROCESS_EXITHANDLER(unicast_close(&unicast);)
	PROCESS_BEGIN();

	// unicast init //
	unicast_open(&unicast, 146, &unicast_callbacks);

	// UART init //
	slip_arch_init(115200);

	//  DATA Storing //
	data_sensing=(uint8_t *) malloc(UART_Full+1 * sizeof(uint8_t));	//size+1 은 첫번째에 indicator 넣기 위함!
	data_sensing[0]='B';	//sensor 식별자 sensor A or sensor B
	//data_sensing[UART_Full]=90;

	while (1) {

		etimer_set(&et, CLOCK_SECOND/10);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

		//printf("%d\t%c\n",data_sensing[0],data_sensing[0]);
		/* RAIDO_LISTEN :
		 * Wait start signal from Sink Node 			*/
		if(CURRENT_STATE==RAIDO_LISTEN){
			PRINTF("CURRENT_STATE: %d. RAIDO_LISTEN \n",CURRENT_STATE);
			leds_on(RAIDO_LISTEN);
			if (RADIO_Input=='S'){
				UART_out('S');
				CURRENT_STATE=UART_START;
				RADIO_Input=NULL;
			}

		}
		/* UART_SEND :
		 * 1. Sensor Interface로 부터 Sensing Complete(C) 신호를 받은 경우
		 * 2. Sensor Interface로 부터 1회 UART 전송을 완료하고 SUCCESS신호를 받은경우
		 *  -> NEXT(N) 신호를 전달하여 다음 데이터 요구
		 * 1. Sensor Interface로 부터 (!SUCCESS)를 받은 경우
		 *  ->Retransmission(R) 신호 전달 */
		else if(CURRENT_STATE==UART_SEND){
			PRINTF("CURRENT_STATE: %d. UART_SEND \n",CURRENT_STATE);
			leds_on(UART_SEND);
			if(UART_Success==1)
				UART_out('N');
			else if (UART_Success==0)
				UART_out('R');

			CURRENT_STATE=UART_LISTEN;
		}
		/* UART_READY :
		 * 1회 전송 가능 data 량을 Sensor interface 로 부터 받은 경우
		 * data를 packet화 하여 unicast 전송 */
		else if(CURRENT_STATE==UART_READY){
			PRINTF("CURRENT_STATE: %d. UART_READY \n",CURRENT_STATE);
			leds_on(UART_READY);

			//헤더 붙이고, 데이터 패킷화 하여 전송하기
			packetbuf_copyfrom(data_sensing, UART_Full+1);
			addr.u8[0] = 4;	addr.u8[1] = 0;	//send to F2
			unicast_send(&unicast, &addr);
			CURRENT_STATE=RADIO_SENT;
		}
		else if(CURRENT_STATE==RADIO_SENT){
			RADIO_NumSend++;
			PRINTF("CURRENT_STATE: %d. RADIO_SNET \t RADIO_NumSend : %d\n",CURRENT_STATE,RADIO_NumSend);
			leds_on(RADIO_SENT);

			if(RADIO_NumSend>=END_SENSING)//End 인 경우
			{
				UART_End=1;	//이건 아마 필요없는 기능이 될듯
				CURRENT_STATE=RAIDO_LISTEN;
				RADIO_NumSend=0;
			}
			else{
				CURRENT_STATE=UART_SEND;
			}
		}
		else if(CURRENT_STATE==UART_START){
			PRINTF("CURRENT_STATE: %d. UART_START \n",UART_START);
			leds_on(UART_START);

		}
		else if(CURRENT_STATE==UART_LISTEN){
			PRINTF("CURRENT_STATE: %d. UART_LISTEN \n",UART_LISTEN);
			leds_on(UART_LISTEN);

		}
		else {
			PRINTF("NONE STATE \t%d\n",CURRENT_STATE);
			leds_on(0);

		}
	}
PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static void recv_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	char* temp_string = (char *) packetbuf_dataptr();
	if(CURRENT_STATE==RAIDO_LISTEN)
		RADIO_Input =  temp_string[0];
	PRINTF("unicast message received from %d.%d\t '%s'\n", from->u8[0], from->u8[1],temp_string );
}

static void sent_uc(struct unicast_conn *c, const rimeaddr_t *from) {
	PRINTF("sent from %d.%d  to  %d.%d\n", rimeaddr_node_addr.u8[0],
		rimeaddr_node_addr.u8[1],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[0],
		packetbuf_addr(PACKETBUF_ADDR_RECEIVER)->u8[1]);
}
int UART_out(char out){
	slip_arch_writeb(out);
	return 1;
}

