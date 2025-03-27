#include "test.h"
#include "../src/cmdproc.h"
#include "../unity/unity.h"


void setUp(void)
{
	int Sensor_Temp=(10, 56,-34 -60, 45, 60, -50, 34,26);
    int Sensor_Humidity=(15,64,89,100,-3,15,48,156,20,0);	
    int Sensor_co2=(400,599,2687,16444,6245,5846,1332,200,60000,85);
	return;
}
void tearDown(void)
{
	return;
}

void test_cmdproc_init(void)
{
	init();
	TEST_ASSERT_EQUAL_INT(0,TxBuffer_length());
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