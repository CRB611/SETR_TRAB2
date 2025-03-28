#include "test.h"
#include "../src/cmdproc.h"
#include "../unity/unity.h"


void setUp(void)
{
	init();
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
	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());

}

void test_command_P(void){
	//temperature
	rxChar('#');	//start
	rxChar('P');	//command	
	rxChar('t');	//argument
	rxChar('1');	//*** 
	rxChar('9');	//checksum
	rxChar('6');	//****
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());

	//humidity
	rxChar('#');	//start
	rxChar('P');	//command	
	rxChar('h');	//argument
	rxChar('1');	//*** 
	rxChar('8');	//checksum
	rxChar('4');	//****
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());

	//c02
	rxChar('#');	//start
	rxChar('P');	//command	
	rxChar('c');	//argument
	rxChar('1');	//*** 
	rxChar('7');	//checksum
	rxChar('9');	//****
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());

}

void test_command_L(void){
	rxChar('#');	//start
	rxChar('L');	//command
	rxChar('0');	//checksum
	rxChar('7');	//checksum
	rxChar('6');	//checksum
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());

}

void test_command_R(void){
	rxChar('#');	//start
	rxChar('R');	//command
	rxChar('0');	//checksum
	rxChar('8');	//checksum
	rxChar('2');	//checksum
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());

}

void test_wrong_checksum(void){
	rxChar('#');	//start
	rxChar('R');	//command
	rxChar('0');	//checksum
	rxChar('4');	//checksum
	rxChar('2');	//checksum
	rxChar('!');	//end

	TEST_ASSERT_EQUAL_INT(-7,cmdProcessor());
}

void teste_txchar(void){
		unsigned char txBuffer[UART_TX_SIZE];
		unsigned char expectBuffer[UART_TX_SIZE];


	for (int i = 0; i < UART_TX_SIZE; i++)
	{
		int dummy = txChar('8');
		if (i < UART_TX_SIZE)
		{
			TEST_ASSERT_EQUAL_INT(END,dummy);

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

	TEST_ASSERT_EQUAL_INT(0,getRxBufferLen());

	rxChar('f');
	rxChar('s');
	rxChar('d');

	TEST_ASSERT_EQUAL_INT(3,getRxBufferLen());

}	


void test_tbuff(void){

	TEST_ASSERT_EQUAL_INT(0,getTxBufferLen());

	txChar('f');
	txChar('s');
	txChar('d');

	TEST_ASSERT_EQUAL_INT(3,getTxBufferLen());

}	
