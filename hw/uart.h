#ifndef _UART_H_
#define _UART_H_

#define USART_REC_LEN  			200  	

extern uint8_t  USART_RX_BUF[USART_REC_LEN]; 
extern uint16_t USART_RX_STA;         		
void uart1_init(uint32_t bound);
void uart6_init(uint32_t bound);



#endif //_UART_H_
