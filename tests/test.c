/**
 * \file test.c
 * \brief This file contains all the functions to make the tests required.
 *
 * In this files are all the functions to test the cmdproc module with UNITY.
 * It includes tests to the initialization, all the commands to process, the conversions required
 * checksums and to buffer related ocurrences.
 * 
 * \author Simão Ribeiro
 * \author Celina Brito
 * \date 8/4/2025
 * \bug There are no known bugs.
 *
 * \defgroup test
 * \brief Functions and structures for testing. 
 * 
 */
#include "test.h"
#include "../src/cmdproc.h"
#include "../unity/unity.h"


static unsigned int size=3;
void setUp(void)
{
	init();
	int t[MAX_SIZE]={50,60,40,35,30,25,20,15,10,0,-10,-15,-20,-25,-30,-35,-40,-50,30,33};
	int h[MAX_SIZE]={50,0,5,10,15,20,25,30,35,40,45,55,60,65,70,75,80,85,90,100};
	int c[MAX_SIZE]={500,2000,3000,4000,5000,6000,7000,8000,9000,10000,11000,12000,13000,14000,15000,16000,17000,18000,19000,20000};

	setValues(t,h,c);
	return;
}

void tearDown(void)
{
	return;
}

void test_cmdproc_init(void)
{
	init();
	TEST_ASSERT_EQUAL_INT(0,getTxBufferLen());
}

void test_command_A(void){
	unsigned char txBuffer[UART_RX_SIZE];

	//normal test, everything is supposed to be alright
	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(OK,cmdProcessor());
	
	//testing the transmission
	int buff_len=getTxBufferLen();
	getTxBuffer(txBuffer,&buff_len);
	unsigned char expected[]={'#','a','t','+','0','5','0','h','0','5','0','c','0','0','5','0','0','1','3','7','!'};

	TEST_ASSERT_EQUAL_CHAR_ARRAY(expected,txBuffer,sizeof(expected));

	//message without start byte
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(SOF_ERROR,cmdProcessor());

	//message without end byte
	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	TEST_ASSERT_EQUAL_INT(EOF_ERROR,cmdProcessor());
}

void test_command_P(void){
	// TEMPERATURA
	rxChar('#');
	rxChar('P');
	rxChar('t');
	rxChar('-');
	rxChar('2');
	rxChar('5');
	rxChar('0');
	rxChar('8');
	rxChar('8');
	rxChar('!');
	TEST_ASSERT_EQUAL_INT(END, cmdProcessor());

	resetRxBuffer();
	resetTxBuffer();

// HUMIDADE
	// Enviar +045 com checksum 124
	rxChar('#');
	rxChar('P');
	rxChar('h');
	rxChar('+');
	rxChar('0');
	rxChar('4');
	rxChar('5');
	rxChar('1');
	rxChar('2');
	rxChar('4');
	rxChar('!');

	TEST_ASSERT_EQUAL_INT(END, cmdProcessor());

	resetRxBuffer();
	resetTxBuffer();

	// CO2
	rxChar('#');
	rxChar('P');
	rxChar('c');
	rxChar('+');
	rxChar('0');
	rxChar('5');
	rxChar('0');
	rxChar('0');
	rxChar('0');
	rxChar('2');  // checksum
	rxChar('1');
	rxChar('1');
	rxChar('!');
	TEST_ASSERT_EQUAL_INT(END, cmdProcessor());

	resetRxBuffer();
	resetTxBuffer();

	// wrong selection
	rxChar('#'); // start
	rxChar('P'); // command
	rxChar('b'); // argument
	rxChar('1'); //***
	rxChar('7'); // checksum
	rxChar('9'); //****
	rxChar('!'); // end
	TEST_ASSERT_EQUAL_INT(NOT_SENSOR, cmdProcessor());
	resetRxBuffer();
	resetTxBuffer();

	//message without start byte
	rxChar('S');
	rxChar('P'); // command
	rxChar('c'); // argument
	rxChar('1'); //***
	rxChar('7'); // checksum
	rxChar('9'); //****
	rxChar('!'); // end
	TEST_ASSERT_EQUAL_INT(SOF_ERROR,cmdProcessor());

	//message without end byte
	rxChar('#');	//start
	rxChar('P');	//command	
	rxChar('c');	//argument
	rxChar('1');	//*** 
	rxChar('7');	//checksum
	rxChar('9');	//****
	TEST_ASSERT_EQUAL_INT(EOF_ERROR,cmdProcessor());
}

void test_command_L(void){
	rxChar('#');	//start
	rxChar('L');	//command
	rxChar('0');	//checksum
	rxChar('7');	//checksum
	rxChar('6');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(END,cmdProcessor());

	//message without start byte
	rxChar('L');	//command
	rxChar('0');	//checksum
	rxChar('7');	//checksum
	rxChar('6');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(SOF_ERROR,cmdProcessor());

	//message without end byte
	rxChar('#');	//start
	rxChar('L');	//command
	rxChar('0');	//checksum
	rxChar('7');	//checksum
	rxChar('6');	//checksum
	TEST_ASSERT_EQUAL_INT(EOF_ERROR,cmdProcessor());

}

void test_command_R(void){
	rxChar('#');	//start
	rxChar('R');	//command
	rxChar('x');	//dummy
	rxChar('x');	//dummy 
	rxChar('x');	//dummy  
	rxChar('0');	//checksum
	rxChar('8');	//checksum
	rxChar('2');	//checksum
	rxChar('!');	//enD
	TEST_ASSERT_EQUAL_INT(END,cmdProcessor());

	//message without start byte
	rxChar('R');	//command
	rxChar('0');	//checksum
	rxChar('8');	//checksum
	rxChar('2');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(SOF_ERROR,cmdProcessor());

	//message without end byte
	rxChar('#');	//start
	rxChar('R');	//command
	rxChar('0');	//checksum
	rxChar('8');	//checksum
	rxChar('2');	//checksum
	TEST_ASSERT_EQUAL_INT(EOF_ERROR,cmdProcessor());
}

void test_nonexistent_cmd(void){
	rxChar('#');	//start
	rxChar('C');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(INV_COMM,cmdProcessor());
}

void test_user_cmd(void){
	char com[6];
	printf("Insert the correct message (#A065!): ");
	scanf("%s",com);
	for (long unsigned int i = 0; i < sizeof(com); i++)
	{
		rxChar(com[i]);
	}

	TEST_ASSERT_EQUAL_INT(OK,cmdProcessor());
}

void test_wrong_values(void) {

	// Temperatura fora do intervalo (-60)
	// Mensagem: #Pt-60xxxCHK!
	// CHK = checksum('P','t','-','6','0') = 88
	rxChar('#');
	rxChar('P');
	rxChar('t');
	rxChar('-');
	rxChar('6');
	rxChar('0');
	rxChar('0'); // chk = 87 
	rxChar('8');
	rxChar('7');
	rxChar('!');
	TEST_ASSERT_EQUAL_INT(VALUES_ERROR, cmdProcessor());

	// Humidade fora do intervalo (150)
	// Mensagem: #Ph+150xxxCHK!
	// CHK = checksum('P','h','+','1','5','0') = 92
	rxChar('#');
	rxChar('P');
	rxChar('h');
	rxChar('+');
	rxChar('1');
	rxChar('5');
	rxChar('0');
	rxChar('1'); // chk = 121 
	rxChar('2');
	rxChar('1');
	rxChar('!');
	TEST_ASSERT_EQUAL_INT(VALUES_ERROR, cmdProcessor());
		// CO2 fora do intervalo (50)
	rxChar('#');
	rxChar('P');
	rxChar('c');
	rxChar('+');
	rxChar('0');
	rxChar('0');
	rxChar('0');
	rxChar('5');
	rxChar('0');
	rxChar('2'); // checksum: 2
	rxChar('1'); // checksum: 1
	rxChar('1'); // checksum: 1
	rxChar('!');
	TEST_ASSERT_EQUAL_INT(VALUES_ERROR, cmdProcessor());
}

void test_wrong_checksum(void){
	//A commands
	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('6');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(CHECKSUM_BAD,cmdProcessor());

	//P command
	rxChar('#');	//start
	rxChar('P');	//command	
	rxChar('t');	//argument
	rxChar('1');	//*** 
	rxChar('2');	//checksum
	rxChar('6');	//****
	rxChar('!');	//end
	rxChar('\0');	//end
	TEST_ASSERT_EQUAL_INT(CHECKSUM_BAD,cmdProcessor());
	
	//R command
	rxChar('#');	//start
	rxChar('R');	//command
	rxChar('0');	//checksum
	rxChar('4');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(CHECKSUM_BAD,cmdProcessor());

	//L command
	rxChar('#');	//start
	rxChar('L');	//command
	rxChar('0');	//checksum
	rxChar('7');	//checksum
	rxChar('0');	//checksum
	rxChar('!');	//end
	TEST_ASSERT_EQUAL_INT(CHECKSUM_BAD,cmdProcessor());
}

void teste_txchar(void){
		unsigned char txBuffer[UART_TX_SIZE];
		unsigned char expectBuffer[UART_TX_SIZE];


	for (int i = 0; i < UART_TX_SIZE; i++)
	{
		int dummy = txChar('8');
		if (i < UART_TX_SIZE)
		{
			TEST_ASSERT_EQUAL_INT(OK,dummy);

			expectBuffer[i]='8';
		}else{

			TEST_ASSERT_EQUAL_INT(FULL_BUFF,dummy);
		}
		
	}
	int len = getTxBufferLen(); // getTxBufferLen() devolve um int
	getTxBuffer(txBuffer, &len); // &len é um int*
	TEST_ASSERT_EQUAL_STRING_LEN(expectBuffer,txBuffer,UART_TX_SIZE);

	resetTxBuffer();

	TEST_ASSERT_EQUAL_INT(0,getTxBufferLen());
}

void teste_rxchar(void){
	unsigned char rxBuffer[UART_RX_SIZE];
	unsigned char expectBuffer[UART_RX_SIZE];


	for (int i = 0; i < UART_RX_SIZE; i++)
	{
		int dummy = rxChar('8');
		if (i < UART_RX_SIZE)
		{
			TEST_ASSERT_EQUAL_INT(OK,dummy);

			expectBuffer[i]='8';
		}else{

			TEST_ASSERT_EQUAL_INT(FULL_BUFF,dummy);
		}
		
	}
	int len = getRxBufferLen(); // getTxBufferLen() devolve um int
	getRxBuffer(rxBuffer, &len); // &len é um int*
	TEST_ASSERT_EQUAL_STRING_LEN(expectBuffer,rxBuffer,UART_TX_SIZE);

	resetRxBuffer();

	TEST_ASSERT_EQUAL_INT(0,getRxBufferLen());
}

void test_num2char(void){
	
	int a=123, b=-34, c=50003; 
	
	unsigned char result[6];

	num2char(&result[0],a,'h');
	TEST_ASSERT_EQUAL_CHAR_ARRAY("123",&result,3);

	num2char(&result[0],a,'t');
	TEST_ASSERT_EQUAL_CHAR_ARRAY("+123",&result,4);

	num2char(&result[0],b,'t');
	TEST_ASSERT_EQUAL_CHAR_ARRAY("-034",&result,4);

	num2char(&result[0],c,'c');
	TEST_ASSERT_EQUAL_CHAR_ARRAY("50003",&result,5);

	num2char(&result[0],a,'c');
	TEST_ASSERT_EQUAL_CHAR_ARRAY("00123",&result,5);
}

void test_char2num(void){

	unsigned char number[3]={'1','2','3'};

	unsigned int numberint=char2num(number,3);

	TEST_ASSERT_EQUAL_INT(123,numberint);
}

void test_rbuff(void){
	unsigned char rxBuffer[UART_RX_SIZE];
	
	//checking if it starts at 0
	TEST_ASSERT_EQUAL_INT(0,getRxBufferLen());

	rxChar('f');
	rxChar('s');
	rxChar('d');
	rxChar('\0');
	
	//testing it the length is right
	TEST_ASSERT_EQUAL_INT(4,getRxBufferLen());

	int buff_len=getRxBufferLen();
	getRxBuffer(rxBuffer,&buff_len);
	
	//testing it the content is right
	TEST_ASSERT_EQUAL_CHAR_ARRAY("fsd\0",rxBuffer,4);

	for (int i = buff_len; i < UART_RX_SIZE+3; i++)
	{
		//adding to buffer while it isn't full
		if (i < UART_RX_SIZE)
		{
			TEST_ASSERT_EQUAL_INT(OK,rxChar('x'));
		
		//adding to buffer while it is full
		}else{
			TEST_ASSERT_EQUAL_INT(FULL_BUFF,rxChar('x'));
		}	
	}

	buff_len=getRxBufferLen();
	TEST_ASSERT_EQUAL_INT(UART_RX_SIZE,buff_len);
	
}	

void test_tbuff(void){
	unsigned char txBuffer[UART_TX_SIZE];
	TEST_ASSERT_EQUAL_INT(0,getTxBufferLen());

	txChar('f');
	txChar('s');
	txChar('d');
	txChar('\0');

	TEST_ASSERT_EQUAL_INT(4,getTxBufferLen());
	
	int buff_len=getTxBufferLen();
	getTxBuffer(txBuffer,&buff_len);
	
	TEST_ASSERT_EQUAL_CHAR_ARRAY("fsd\0",txBuffer,4);

	
	for (int i = buff_len; i < UART_TX_SIZE+3; i++)
	{
		//adding to buffer while it isn't full
		if (i < UART_TX_SIZE)
		{
			TEST_ASSERT_EQUAL_INT(OK,txChar('x'));
		
		//adding to buffer while it is full
		}else{
			TEST_ASSERT_EQUAL_INT(FULL_BUFF,txChar('x'));
		}	
	}

	buff_len=getTxBufferLen();
	TEST_ASSERT_EQUAL_INT(UART_TX_SIZE,buff_len);
	
}	

void test_addValue(void){
	int a[MAX_SIZE]={1,2,3};
	int expected[MAX_SIZE]={6,1,2,3};
	addValue(a,&size,6);

	TEST_ASSERT_EQUAL_INT_ARRAY(expected,a,4);
	
	int b[MAX_SIZE]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	int expectedb[MAX_SIZE]={14,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
	
	addValue(b,&size,14);

	TEST_ASSERT_EQUAL_INT_ARRAY(expectedb,b,4);	
}

void test_getsensor(void){

	int tfirst_exp=50, cfirst_exp=500, hfirst_exp=50;
	int tfirst=getFirstTemp();
	int hfirst=getFirstHum();
	int cfirst=getFirstco2();

	TEST_ASSERT_EQUAL_INT(tfirst_exp,tfirst);
	TEST_ASSERT_EQUAL_INT(hfirst_exp,hfirst);
	TEST_ASSERT_EQUAL_INT(cfirst_exp,cfirst);

}
