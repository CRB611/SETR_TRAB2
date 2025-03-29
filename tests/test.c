#include "test.h"
#include "../src/cmdproc.h"
#include "../unity/unity.h"


static unsigned int size=3;
void setUp(void)
{
	init();
	int t[MAX_SIZE]={50};
	int h[MAX_SIZE]={50};
	int c[MAX_SIZE]={500};

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

	TEST_ASSERT_EQUAL_INT(0,cmdProcessor());
	
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

	/* not working for now
	printf("\nnoend");
	//message without end byte
	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	
	TEST_ASSERT_EQUAL_INT(EOF_ERROR,cmdProcessor());
*/
	
	int t[MAX_SIZE]={-60};
	int h[MAX_SIZE]={50};
	int c[MAX_SIZE]={500};
	setValues(t,h,c);

	//unnaccepted temp value
	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	
	TEST_ASSERT_EQUAL_INT(VALUES_ERROR,cmdProcessor());

	//unnaccepted humidity value
	int t2[MAX_SIZE]={30};
	int h2[MAX_SIZE]={500};
	int c2[MAX_SIZE]={500};
	setValues(t2,h2,c2);

	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	
	TEST_ASSERT_EQUAL_INT(VALUES_ERROR,cmdProcessor());

	
	//unnaccepted co2 value
	int t3[MAX_SIZE]={30};
	int h3[MAX_SIZE]={50};
	int c3[MAX_SIZE]={50};
	setValues(t3,h3,c3);

	rxChar('#');	//start
	rxChar('A');	//command
	rxChar('0');	//checksum
	rxChar('6');	//checksum
	rxChar('5');	//checksum
	rxChar('!');	//end
	
	TEST_ASSERT_EQUAL_INT(VALUES_ERROR,cmdProcessor());

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

void test_command_P(void){
	//temperature
	rxChar('#');	//start
	rxChar('P');	//command	
	rxChar('t');	//argument
	rxChar('1');	//*** 
	rxChar('9');	//checksum
	rxChar('6');	//****
	rxChar('!');	//end
	rxChar('\0');	//end

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
