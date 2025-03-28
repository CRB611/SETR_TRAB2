/* ******************************************************/
/* SETR 23/24, Paulo Pedreiras                          */
/* 		Sample code for Assignment 2					*/
/*   	A few tests to the cmdProcessor to illustrate	*/
/*      how the the tests can be carried out.         	*/
/*														*/
/* Note that I'm not using Unity! That is part of your 	*/
/*		work. 											*/
/*                                                      */
/* Compile with: gcc cmdproc.c main.c -o main           */
/*	Feel free to use flags such as -Wall -Wpedantic ...	*/
/*	and it is a good idea to create a makefile			*/
/*                                                      */
/* ******************************************************/
#include <stdio.h>
#include <string.h>
#include "cmdproc.h"
#include "../unity/unity.h"
#include "teste.h"

void teste_txchar(void){
	unsigned char txBuffer[UART_TX_SIZE];
	unsigned char expectBuffer[UART_TX_SIZE];


for (int i = 0; i < UART_TX_SIZE; i++)
{
	int dummy = txChar('8');
	if (i < UART_TX_SIZE)
	{
		TEST_ASSERT_EQUAL_INT(0,dummy);

		expectBuffer[i]='8';
	}else{

		TEST_ASSERT_EQUAL_INT(-1,dummy);
	}
	
}
getTxBuffer(txBuffer,getTxBufferLen());
TEST_ASSERT_EQUAL_STRING_LEN(expectBuffer,txBuffer,UART_TX_SIZE);

resetTxBuffer();

TEST_ASSERT_EQUAL_INT(0,getTxBufferLen());
}





/*
int main(void) 
{
	int i,len, err;
	unsigned char ans[30]; 
	unsigned char ansTest1[]={'#','p','t', '+', '2', '1', '1', '1', '4','!'};
	
	int temp_int= 123, humid_int=67, co2_int=6000;
	char *temp= num2char(temp_int,'t');
	char *Humid= num2char(humid_int,'a');
	char *co2= num2char(co2_int,'a');

	unsigned char *message;

	printf("%d .. t= %s , h=%s, c=%s",4+sizeof(temp)+sizeof(Humid)+sizeof(co2),temp,Humid,co2);
	/*
	for (int i = 0; i < 4+sizeof(temp)+sizeof(Humid)+sizeof(co2); i++)
	{
		
	}
	

	printf("\n %s \n ",message);

	printf("\n Smart Sensor interface emulation \n");
	printf(" \t - simple illustration of interface and use \n\n\r");
	
	/* Init UART RX and TX buffers 
	resetTxBuffer();
	resetRxBuffer();
	
	/* Test 1 
	
	printf("Test1 - check the answer to a valid Pt command\n");
	
	/* 1 - send the command 
	rxChar('#');
	rxChar('P');
	rxChar('t');
	rxChar('1');
	rxChar('9');
	rxChar('6');
	rxChar('!');
			
	/* 2 - Process the comand and check the answer 
	
	cmdProcessor();
	
	getTxBuffer(ans,&len);
	if(memcmp(ans,ansTest1,len)) {
		printf("Test 1 failed\n");
	} else {
		printf("Test 1 succeeded\n");
	}	
	
	/* You can print the answer to see what is wrong, if necessary 
	printf("\t Received answer:");
	for(i=0; i < len; i++) {
		printf("%c", ans[i]);
	}
	printf("\n\t Expected answer:");
	i=sizeof(ansTest1);
	for(i=0; i< len; i++) {
		printf("%c", ansTest1[i]);
	}
	printf("\n");
	

	/* Test 2 
	
	printf("Test2 - check the answer to a transmission omission/error \n");
	
	/* 1 - send the command 
	rxChar('#');
	rxChar('P');
	// rxChar('t'); - simulates missing character, emulates a tx error 
	rxChar('1');
	rxChar('9');
	rxChar('6');
	rxChar('!');
			
	/* 2 - Process the comand and check the answer 
	
	err=cmdProcessor();
		
	if(err == -2) {
		printf("Test 2 succeeded, as omission was detected\n");
	} else {
		printf("Test 2 failed, as omission was not detected\n");
	}		
	
	/* Much more tests are needed. Unity shoul be used for it. 
	
	return 0;
}
*/