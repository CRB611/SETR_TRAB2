/**
 * \file cmdproc.c
 * \brief This file contains all the functions needed for the C module.
 *
 *  This module processes commands received via UART, one character at a time. 
 * The module is part of a smart sensor that reads:
 * <ul>
    <li>Temperature, from -50ºC to 60ºC;</li>
    <li>Relative humidity, from 0% to 100%;</li>
    <li>CO<sub>2</sub>, from 400 to 2000 ppm;</li>
</ul>
 *  In this file there are functions to initialize the module, to manage the transmission and reception buffers, 
 * to get the information from the sensors and to do the necessary conversions to process the ASCII messages.
 *  This file also has some functions required to emulate the UART.
 * This file is based on the source file given to us by professor Paulo Pedreiras. 
 * \author Simão Ribeiro
 * \author Celina Brito
 * \date 1/4/2025
 * \bug There are no known bugs.
 * \defgroup C Module
 * \brief Functions and structures for the C module. 
 *
 */ 
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
static unsigned int txBufLen = 0; 
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
		return OK;		
	}	
	/* If cmd string full return error */
	return FULL_BUFF;
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
		return OK;		
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


void clearRXBuffer(int len){
	//taking the data out of the buffer
	memmove(UARTRxBuffer ,UARTRxBuffer + len, rxBufLen - len);
	
	//reseting the length
	rxBufLen -= len;

	//setting the 1st "character" as "\0"
	UARTTxBuffer[rxBufLen]='\0';
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


void get_temp(int *t){
	*t=*temp;
}
void get_hum(int *h){
	*h=*hum;
}
void get_co2(int *c){
	*c=*co2;
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
Seting the sensor arrays for emulation porposes
*/
void setValues(int t[MAX_SIZE], int h[MAX_SIZE], int c[MAX_SIZE]){
	for (int i = 0; i < MAX_SIZE; i++)
	{
		temp[i] = t[i];
		hum[i] = h[i];
		co2[i] = c[i];
	}
}


/*
Errase RX BUFF
*/
void eraseRxBuff(int len){

	//sets everything to \0
	for (int i = 0; i < len; i++)
	{
		UARTRxBuffer[i]='\0';
	}
	rxBufLen-=len;
	UARTRxBuffer[rxBufLen] = '\0';
}
/*
Errase TX BUFF
*/
void eraseTxBuff(int len){

	//sets everything to \0
	for (int i = 0; i < len; i++)
	{
		UARTTxBuffer[i]='\0';
	}
	//resets the length
	txBufLen-=len;
	UARTTxBuffer[txBufLen] = '\0';
}


/* 
 * cmdProcessor
 */ 
int cmdProcessor(void)
{
	unsigned int i=0,k=0;
	unsigned char sid;
		
	/* Detect empty cmd string */
	if(rxBufLen == 0)
		return EMPTY; 

	/* Find index of SOF */
	for(i=0; i < rxBufLen; i++) {
		if(UARTRxBuffer[i] == SOF_SYM) {
			break;
		}else if (i == (unsigned int)rxBufLen-1){
			eraseRxBuff(rxBufLen);
			return SOF_ERROR;
		}
	}
	
	/* If a SOF was found look for commands */
	if(i < rxBufLen) {
		
		switch(UARTRxBuffer[i+1]) { 
			
			case 'P':	
			{	
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
					// Verifica se a mensagem termina corretamente
					for (k = 0; k <= MAX_SIZE; k++)
					{
						if (UARTRxBuffer[k] == EOF_SYM)
						{
							break;
							
						// Se não acaba com o simbolo que deve dá erro
						}else if (k == MAX_SIZE-1){
							eraseRxBuff(rxBufLen);
							return EOF_ERROR;
						}
					}
					
					/* Check checksum */
					int chk = calcChecksum(&UARTRxBuffer[i+1], k-4); // inclui o tipo, sinal e valor
					int chk_recv = char2num(&UARTRxBuffer[k-3], 3); // os três dígitos ASCII
				
					//verificar a checksum
					if (chk != chk_recv) {
						return CHECKSUM_BAD;
					}
					
					txChar('#');
					txChar('P');
					txChar('t');

					//int c_n = char2num(&UARTRxBuffer[i+4],2);
					unsigned char temp[3];
					int n= getFirstTemp();
					num2char(&temp[0],n,'t');
					
					if (n>60 || n<-50)
					{
						eraseRxBuff(rxBufLen);
						return VALUES_ERROR;
					}
					
					for(int j =0;j<3;j++){
						txChar(temp[j]);
					}

					int check= calcChecksum(&UARTTxBuffer[2],16);
					unsigned char check_c[3];
					num2char(&check_c[0],check,'h');

					for(int j = 0;j<3;j++){
						txChar(check_c[j]);
					}
					txChar('!');
					
					eraseRxBuff(rxBufLen);
					return END;

				}
				else if(sid=='h'){

					// Verifica se a mensagem termina corretamente
					for (k = 0; k <= MAX_SIZE; k++)
					{
						if (UARTRxBuffer[k] == EOF_SYM)
						{
							break;
							
						// Se não acaba com o simbolo que deve dá erro
						}else if (k == MAX_SIZE-1){
							eraseRxBuff(rxBufLen);
							return EOF_ERROR;
						}
					}

					/* Check checksum */
					int chk = calcChecksum(&UARTRxBuffer[i+1], k-4); // inclui o tipo, sinal e valor
					int chk_recv = char2num(&UARTRxBuffer[k-3], 3); // os três dígitos ASCII
				
					//verificar a checksum
					if (chk != chk_recv) {
						return CHECKSUM_BAD;
					}
					
					txChar('#');
					txChar('P');
					txChar('h');

					unsigned char hum[3];
					int h= getFirstHum();
					num2char(&hum[0],h,'h');

					if (h>100 || h<0)
					{
						eraseRxBuff(rxBufLen);
						return VALUES_ERROR;
					}

					for(int j =0;j<3;j++){
						txChar(hum[j]);
					}

					int check= calcChecksum(&UARTTxBuffer[2],16);
					unsigned char check_c[3];
					num2char(&check_c[0],check,'h');

					for(int j = 0;j<3;j++){
						txChar(check_c[j]);
					}
					txChar('!');

					eraseRxBuff(rxBufLen);
					return END;
				}
				else if (sid == 'c') {
					// Verifica se a mensagem termina corretamente
					for (k = 0; k <= MAX_SIZE; k++)
					{
						if (UARTRxBuffer[k] == EOF_SYM)
						{
							break;
							
						// Se não acaba com o simbolo que deve dá erro
						}else if (k == MAX_SIZE-1){
							eraseRxBuff(rxBufLen);
							return EOF_ERROR;
						}
					}
					
					/* Check checksum */
					int chk = calcChecksum(&UARTRxBuffer[i+1], k-4); // inclui o tipo, sinal e valor
					int chk_recv = char2num(&UARTRxBuffer[k-3], 3); // os três dígitos ASCII
					
					//verificar a checksum
					if (chk != chk_recv) {
						return CHECKSUM_BAD;
					}
					
					txChar('#');
					txChar('P');
					txChar('c');
					
					unsigned char  co2[5];
					int n= getFirstco2();
					num2char(&co2[0],n,'c');
					
					if (n < 400 || n > 20000) {
						eraseRxBuff(rxBufLen);
						return VALUES_ERROR;
					}
				
					for(int j =0;j<5;j++){
						txChar(co2[j]);
					}

					int check= calcChecksum(&UARTTxBuffer[2],16);
					unsigned char check_c[3];
					num2char(&check_c[0],check,'h');

					for(int j = 0;j<3;j++){
						txChar(check_c[j]);

					}
					txChar('!');
					


					return END;

				}
				else{
					eraseRxBuff(rxBufLen);
					return INV_COMM;
				}
			}
			case 'A':
			{
				int T, H, C;
				unsigned char T_char[3], H_char[3], C_char[5];
			
				// Verifica se a mensagem termina corretamente
				for (k = 0; k <= MAX_SIZE; k++)
				{
					if (UARTRxBuffer[k] == EOF_SYM)
					{
						break;
						
					// Se não acaba com o simbolo que deve dá erro
					}else if (k == MAX_SIZE-1){
						eraseRxBuff(rxBufLen);
						return EOF_ERROR;
					}
			
				}
			
				// Calcula e compara o checksum
				int chk = calcChecksum(&UARTRxBuffer[i+1], k-4);  // de 'A' até ao fim do CO2
				int chk_recv = char2num(&UARTRxBuffer[k-3], 3); // posições do fim do c02 até '!'
			
				if (chk != chk_recv) {
					return CHECKSUM_BAD;
				}
			
				//mensagem a ser devolvida
				txChar('#');	//SOF byte	
				txChar('a');	//resposta a A

				//obter os resultados
				T = getFirstTemp();
				H = getFirstHum();
				C = getFirstco2();

				//verificar se estão nos intervalos
				if ((T < -50 || T > 60))
				{
					eraseRxBuff(rxBufLen);
					return VALUES_ERROR;
				}else if ((H < 0 || H > 100))
				{
					eraseRxBuff(rxBufLen);
					return VALUES_ERROR;
				}else if ((C < 400 || C > 20000))
				{
					eraseRxBuff(rxBufLen);
					return VALUES_ERROR;
				}
				
				//converter para char
				num2char(&T_char[0],T,'t');
				num2char(&H_char[0],H,'h');
				num2char(&C_char[0],C,'c');

				//inserir dados
				txChar('t');
				for (int j = 0; j < 3; j++)
				{
					txChar(T_char[j]);
				}

				txChar('h');
				for (int j = 0; j < 3; j++)
				{
					txChar(H_char[j]);
				}

				txChar('c');	
				for (int j = 0; j < 5; j++)
				{
					txChar(C_char[j]);
				}

				//checksum
				int t_check= calcChecksum(&UARTTxBuffer[2],16);
				unsigned char t_check_c[3];
				num2char(&t_check_c[0],t_check,'h');


				for (int j = 0; j < 3; j++)
				{
					txChar(t_check_c[j]);
				}

				txChar('!');

				eraseRxBuff(rxBufLen);
				return OK;


			
			}	
			case 'L':
			{
				// 1. Verifica se a mensagem termina com '!' no sítio certo
				for (k = 0; k <= MAX_SIZE; k++)
				{
					if (UARTRxBuffer[k] == EOF_SYM)
					{
						break;
						
					// Se não acaba com o simbolo que deve dá erro
					}else if (k == MAX_SIZE-1){
						eraseRxBuff(rxBufLen);
						return EOF_ERROR;
					}
				}
				
				// 2. Checksum: calcula e compara
				int chk = calcChecksum(&UARTRxBuffer[i+1], k-4); // inclui o tipo, sinal e valor
				int chk_recv = char2num(&UARTRxBuffer[k-3], 3); // os três dígitos ASCII
				
				if (chk != chk_recv) {
					return CHECKSUM_BAD;
				}

				//caracteres iniciais
				txChar('#');
				txChar('L');
				txChar('t');

				// 3. Envia as amostras de Temperatura
				int tget[MAX_SIZE];
				get_temp(&tget[0]);

				printf("t");
				for (i = 0; i < 20; i++) {
					unsigned char temp_ascii[3];
					num2char(&temp_ascii[0], temp[i], 't');
					txChar(temp_ascii[0]); // '+' ou '-'
					txChar(temp_ascii[1]);
					txChar(temp_ascii[2]);
				}	
			
				// 4. Envia as amostras de Humidade
				txChar('h');
				int h[MAX_SIZE];
				get_hum(&h[0]);
				
				for (i = 0; i < 20; i++) {
					unsigned char hum_ascii[3];
					num2char(&hum_ascii[0], hum[i], 'h');
					txChar(hum_ascii[0]); // '+' ou '-'
					txChar(hum_ascii[1]);
					txChar(hum_ascii[2]);
				}	

				// 4. Envia as amostras de co2
				txChar('c');
				int c[MAX_SIZE];
				get_co2(&c[0]);
				
				for (i = 0; i < 20; i++) {
					unsigned char co2_ascii[5];
					num2char(&co2_ascii[0], co2[i], 'c');
					txChar(co2_ascii[0]); // '+' ou '-'
					txChar(co2_ascii[1]);
					txChar(co2_ascii[2]);
					txChar(co2_ascii[3]);
					txChar(co2_ascii[4]);
				}	
				txChar('!');

				// 6. Limpa o buffer e termina
				eraseRxBuff(rxBufLen);
				return END;
			}

			case 'R':
			{
				// 1. Verifica EOF
				if (UARTRxBuffer[i + 8] != EOF_SYM)
				{
					eraseRxBuff(rxBufLen);
					return EOF_ERROR;
				}
				if (UARTRxBuffer[i] != SOF_SYM)
				{
					eraseRxBuff(rxBufLen);
					return SOF_ERROR;
				}

				// 2. Verifica checksum
				int chk = calcChecksum(&UARTRxBuffer[i + 1], 1);
				int chk_recv = char2num(&UARTRxBuffer[i + 5], 3);
				if (chk != chk_recv)
					return CHECKSUM_BAD;

				// 3. Reset aos índices
				index_temp = 0;
				index_hum = 0;
				index_co2 = 0;

				// 4. Limpar os arrays
				for (int i = 0; i < MAX_SIZE; i++)
				{
					temp[i] = 0;
					hum[i] = 0;
					co2[i] = 0;
				}

				// 5. Ecoa a mensagem original
				for (int i = 0; i < rxBufLen; i++)
					txChar(UARTRxBuffer[i]);

				// 6. Envia o checksum
				unsigned char chk_ascii[3];
				num2char(chk_ascii, chk, 'c'); // 3 dígitos ASCII
				txChar(chk_ascii[0]);
				txChar(chk_ascii[1]);
				txChar(chk_ascii[2]);

				// 7. Limpa e termina
				eraseRxBuff(rxBufLen);
				return END;
			}
			default:
			{
				eraseRxBuff(rxBufLen);
				return INV_COMM;	
			}				
		}
		
		
	}else
	
	/* Cmd string not null and SOF not found */
	return FATAL_ERROR;

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
		
		len=2;

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



