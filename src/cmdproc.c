/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include <stdio.h>
#include <string.h>
#include <math.h> 

#include "cmdproc.h"

/* Internal variables */
/* Used as part of the UART emulation */
static unsigned char UARTRxBuffer[UART_RX_SIZE];
static unsigned char rxBufLen = 0; 

static unsigned char UARTTxBuffer[UART_TX_SIZE];
static unsigned char txBufLen = 0; 
static unsigned  index_hum; 
static int hum[MAX_SIZE];
static unsigned  index_temp; 
static int temp[MAX_SIZE];
static unsigned  index_co2; 
static int co2[MAX_SIZE];
/* Function implementation */


/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
	unsigned int sum = 0 ;
	
	for(unsigned char* i = buf ; i < (buf+nbytes);i++ ){
		sum += (unsigned int)(*i);
	}
	return (sum%256);		
}

/*
 * rxChar
 */
int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufLen < UART_RX_SIZE) {
		UARTRxBuffer[rxBufLen] = car;
		rxBufLen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
}

/*
 * txChar
 */
int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufLen < UART_TX_SIZE) {
		UARTTxBuffer[txBufLen] = car;
		txBufLen += 1;
		return END;		
	}	
	/* If cmd string full return error */
	return FULL_BUFF;
}

/*
 * resetRxBuffer
 */
void resetRxBuffer(void)
{
	rxBufLen = 0;		
	return;
}

/*
 * resetTxBuffer
 */
void resetTxBuffer(void)
{
	txBufLen = 0;		
	return;
}

/*
 * getTxBuffer
 */
int getTxBuffer(unsigned char * buf, int * len)
{
	if(txBufLen==0){
		return EMPTY;
	}

	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
	return 0;
}

int getRxBuffer(unsigned char * buf, int * len)
{
	if(rxBufLen==0){
		return EMPTY;
	}

	*len = rxBufLen;
	if(rxBufLen > 0) {
		memcpy(buf,UARTRxBuffer,*len);
	}		
	return 0;
}
/*
*getTxBufferLen
*/
int getTxBufferLen(void){

if (txBufLen <= UART_TX_SIZE )
{
	return txBufLen;
}else
	return FULL_BUFF; // Size_error 
}


int clearRXBuffer(int * len){
	return OK;
}

int getRxBufferLen(void){

	if (rxBufLen <= UART_RX_SIZE )
	{
		return rxBufLen;
	}else
		return FULL_BUFF; // Size_error 
	}

/*
*Init function
*/
int init(void){
	index_temp=0;
	index_co2=0;
	index_hum=0;
	for ( int i = 0; i <= MAX_SIZE; i++)
	{
		hum[i]=0;
		temp[i]=0;
		co2[i]=0;
	}
	resetRxBuffer();
	resetTxBuffer();

	if(txBufLen==0){
		return EMPTY;//vazio
	}else{
		return NOT_EMPTY;//Não vazio	
	}
}
/*
Get Temp
*/
int* get_temp(void){

	return temp;
}
/*
Get HUm
*/
int* get_hum(void){
	return hum;
}
/*
Get CO2
*/
int* get_co2(void){
	return co2;
}
/*
Get ALL (Temp,HUm,Co2)
*/
void get_all(){
	get_temp();
	get_hum();
	get_co2();
}

/*
Get Temp first 
*/
int getFirstTemp(void){
	return temp[0];
}
/*
Get Hum first
*/
int getFirstHum(void){
	return hum[0];
}
/*
Get CO2 first
*/
int getFirstco2(void){
	return co2[0];
}




/*
Errase RX BUFF
*/
void erraseRxBuff(int len){

	memmove(UARTRxBuffer,UARTRxBuffer+len,UARTRxBuffer-len);
	rxBufLen-=len;
	UARTRxBuffer[rxBufLen] = '\0';



}
/*
Errase TX BUFF
*/
void erraseTxBuff(int len){

	memmove(UARTTxBuffer,UARTTxBuffer+len,UARTTxBuffer-len);
	txBufLen-=len;
	UARTTxBuffer[txBufLen] = '\0';



}


/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	int i;
	unsigned char sid;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return EMPTY; 
	
	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'P':		
				/* Command "P" detected.							*/
				/* Follows one DATA byte that specifies the sensor	*/ 
				/* to read. I assume 't','h','c' for temp., humid. 	*/
				/* and CO2, resp.									*/   
		
				/* Check sensor type */
				sid = UARTRxBuffer[i+2];
				
				if(sid != 't' && sid != 'h' && sid != 'c') {
					return NOT_SENSOR;
				}


				if ( sid =='t')
				{
					if(UARTRxBuffer[i+9] != EOF_SYM){
						erraseRxBuff(rxBufLen);
						return EOF_ERROR;

					}
				

				
					/* Check checksum */
					int chk = calcChecksum(&UARTRxBuffer[i+1], 5); // inclui o tipo, sinal e valor
					int chk_recv = char2num(&UARTRxBuffer[i+6], 3); // os três dígitos ASCII
					
					if (chk != chk_recv) {
						return CHECKSUM_BAD;
					}
					

					int c_n = char2num(&UARTRxBuffer[i+4],2);

					if(UARTRxBuffer[i+3] == '-'){
						c_n=-c_n;
					}

					if (c_n>60 || c_n<-50)
					{
						erraseRxBuff(rxBufLen);
						return VALUES_ERROR;
					}
					
					addValue(temp,&index_temp,c_n);
					for(int i =0;i<rxBufLen;i++){
						txChar((unsigned char)(UARTRxBuffer[i]));
					}

					num2char(&UARTTxBuffer[txBufLen-3],chk,3);

					erraseRxBuff(rxBufLen);
					return END;

				}
				else if(sid=='h'){

					if(UARTRxBuffer[i+10] != EOF_SYM){
						erraseRxBuff(rxBufLen);
						return EOF_ERROR;

					}
					
					/* Check checksum */
					int chk = calcChecksum(&UARTRxBuffer[i+1], 6); // inclui o tipo, sinal e valor
					int chk_recv = char2num(&UARTRxBuffer[i+7], 3); // os três dígitos ASCII
					
					if (chk != chk_recv) {
						return CHECKSUM_BAD;
					}
					
					int c_n = char2num(&UARTRxBuffer[i+4],3);

					if (c_n>100 || c_n<0)
					{
						erraseRxBuff(rxBufLen);
						return VALUES_ERROR;
					}

					addValue(hum,&index_hum,c_n);
					for(int i =0;i<rxBufLen;i++){
						txChar((unsigned char)(UARTRxBuffer[i]));
					}

					num2char(&UARTTxBuffer[txBufLen-3],chk,3);

					erraseRxBuff(rxBufLen);
					return END;
				}
				else if (sid == 'c') {
					if(UARTRxBuffer[i+12] != EOF_SYM){
						erraseRxBuff(rxBufLen);
						return EOF_ERROR;
					}
				
					int chk = calcChecksum(&UARTRxBuffer[i+1], 7); // 'P', 'c', '+', d1, d2, d3, d4, d5
					int chk_recv = char2num(&UARTRxBuffer[i+9], 3);
				
					if (chk != chk_recv) {
						return CHECKSUM_BAD;
					}
				
					int c_n = char2num(&UARTRxBuffer[i+4], 5); // lê 5 dígitos

				
					if (c_n < 400 || c_n > 20000) {
						erraseRxBuff(rxBufLen);
						return VALUES_ERROR;
					}
				
					addValue(co2, &index_co2, c_n);
				
					for (int j = 0; j < rxBufLen; j++) {
						txChar(UARTRxBuffer[j]);
					}

					num2char(&UARTTxBuffer[txBufLen-3],chk,3);
					erraseRxBuff(rxBufLen);
					return END;

				}
				else{
					erraseRxBuff(rxBufLen);
					return INV_COMM;
				}
			case 'A':
				int T, H, C;
			
				// Verifica se a mensagem termina corretamente
				if (UARTRxBuffer[i+19] != EOF_SYM) {
					erraseRxBuff(rxBufLen);
					return EOF_ERROR;
				}
			
				// Calcula e compara o checksum
				int chk = calcChecksum(&UARTRxBuffer[i+1], 15);  // de 'A' até ao fim do CO2
				int chk_recv = char2num(&UARTRxBuffer[i+16], 3); // posições 16 a 18
			
				if (chk != chk_recv) {
					return CHECKSUM_BAD;
				}
			
				// Ponteiro para percorrer os campos t, h, c
				unsigned char *sid = &UARTRxBuffer[i+2];
			
				while (*sid != EOF_SYM) {
					if (*sid == 't') {
						T = 10 * (*(sid+2) - '0') + (*(sid+3) - '0');
						if (*(sid+1) == '-') T = -T;
			
						if (T < -50 || T > 60) {
							erraseRxBuff(rxBufLen);
							return VALUES_ERROR;
						}
			
						sid += 4; // avançar 4 posições: t + sinal + 2 dígitos
					}
					else if(*sid == 'h'){

						H=100*(*(sid+1)-'0')+10*(*(sid+2)-'0'+(*(sid+3)-'0'));
						
						if (H>100 || H<0)
						{
							erraseRxBuff(rxBufLen);
							return VALUES_ERROR;
						}
					sid+=4;
					
					
					}
					else if(*sid == 'c'){
						C=10000*(*(sid+1)-'0')+1000*(*(sid+2)-'0')+100*(*(sid+3)-'0')+10*(*(sid+4)-'0')+(*(sid+5)-'0');
						
						
						
						if (C < 400 || C > 20000) {
							erraseRxBuff(rxBufLen);
							return VALUES_ERROR;
						}


					sid+=6;	

					} else{
						sid+=3;
						return INV_COMM;
					}
					
				}
				addValue(temp,&index_temp,T);
				addValue(hum,&index_hum,H);
				addValue(co2,&index_co2,C);

				for (int i = 0; i < UARTRxBuffer; i++)
				{
					txChar((unsigned char)(UARTRxBuffer[i]));
				}
				
				num2char(&UARTTxBuffer[txBufLen-3],chk,3);


				erraseRxBuff(rxBufLen);
				return END;






























				/* Command is (is it? ... ) valid. Produce answer and terminate */ 
				txChar('#');
				txChar('p'); /* p is the reply to P 							*/	
				txChar('t'); /* t indicate that it is a temperature 			*/
				txChar('+'); /* This is the sensor reading. You should call a 	*/
				txChar('2'); /*   function that emulates the reading of a 		*/
				txChar('1'); /*   sensor value 	*/
				txChar('1'); /* Checksum is 114 decimal in this case		*/
				txChar('1'); /*   You should call a funcion that computes 	*/
				txChar('4'); /*   the checksum for any command 				*/  
				txChar('!');
				
				/* Here you should remove the characters that are part of the 		*/
				/* command from the RX buffer. I'm just resetting it, which is not 	*/
				/* a good solution, as a new command could be in progress and		*/
				/* resetting  will generate errors									*/
				rxBufLen = 0;	
				
				return OK;


			default:
				/* If code reaches this place, the command is not recognized */
				return INV_COMM;				
		}
		
		
	}
	
	/* Cmd string not null and SOF not found */
	return -4;

}

void num2char(unsigned char *array, int num, char type){
    int i = 0;
	int len=3;

	//checking what type of data it is
	if (type=='t')
	{
		if (num>=0)
		{
			*array='+';
		}else{
			*array='-';
		}
		num=abs(num);
		
		while (i < len) {
			*(array + len-i) = (num % 10) + '0';
			num /= 10;
			i++;
		} 
		return;

	}else if (type=='c'){
		len=5;
	}

	while (i < len) {
		*(array + len-i-1) = (num % 10) + '0';
		num /= 10;
		i++;
	} 
	
    
}

unsigned int char2num(unsigned char ascii [], int length){
	int i = 0, sum = 0, mult = pow(10,length-1);
	int x;
	
	while(i < length){
		x= (ascii[i]-'0') *mult ;

	  	sum += x;
		mult/=10;
	  	i++;
	}
	return sum;
}

void addValue(int *array, unsigned int *size, int value){
	
	//shifting everything +1
	for(unsigned int i = *size; i > 0; i--) {
        array[i] = array[i-1];
    }
    array[0] = (unsigned int)value; 
	
    if(*size < MAX_SIZE-1) {
        (*size)++;
    }

}



