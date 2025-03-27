/* ****************************** */
/* See cmdProc.h for indications  */
/* ****************************** */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
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
void getTxBuffer(unsigned char * buf, int * len)
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
/*
*getTxBufferLen
*/
int getTxBufferLen(void){

if (txBufLen <= UART_TX_SIZE )
{
	return;
}else
	return FULL_BUFF; // Size_error 

/*
*Init function
*/
}
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
	if(sizeof(temp)==0){
		return; //tmep vazio
	}
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
				
				/* Check checksum */
				if(!(calcChecksum(&(UARTRxBuffer[i+1]),2))) {
					return CHECKSUM_BAD;
				}
				
				/* Check EOF */
				if(UARTRxBuffer[i+6] != EOF_SYM) {
					return WRONG_FORMAT;
				}
			
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
<<<<<<< HEAD

/* 
 * calcChecksum
 */ 
int calcChecksum(unsigned char * buf, int nbytes) {
	unsigned int sum = 0 ;
	
	for(unsigned char* i = buf ; i < (buf+nbytes);i++ ){
		sum += (unsigned int)(*i);
	}
	return (sum%127);		
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
		return 0;		
	}	
	/* If cmd string full return error */
	return -1;
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
void getTxBuffer(unsigned char * buf, int * len)
{
	*len = txBufLen;
	if(txBufLen > 0) {
		memcpy(buf,UARTTxBuffer,*len);
	}		
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

char* num2char(int num,char type){

	char number[6],temp[6];

	if (type == 't')
	{
	
		int i=1;

		//checking if its a negative number
		if (num < 0)
		{
			number[0]='-';		
		}else{
			number[0]='+';
		}
		
		num=abs(num);

		while (num >= 10)
		{
			number[i] = (char)(num % 10) + '0';
			temp[i]=number[i];
			i++;
			num=num/10;
		}
		number[i]=(char)num+'0';
		temp[i]=number[i];
		

		int count=i;
		for (int k = 1; k <= count; k++)
		{
			number[k]=temp[i];
			i--;	
		}
		
		return number;
	}else{
		int i=0;

		while (num >= 10)
		{
			number[i] = (char)(num % 10) + '0';
			temp[i]=number[i];
			i++;
			num=num/10;
		}
		number[i]=(char)num+'0';
		temp[i]=number[i];
		
		int count=i;
		for (int k = 1; k <= count; k++)
		{
			number[k]=temp[i];
			i--;	
		}
		
		return number;
	}
}

int getCO2(void){
	return 0;
}

int getHumidity(void){
	return 0;

}

int getTemp(void){
	return 0;

}

=======
>>>>>>> branch_simao
