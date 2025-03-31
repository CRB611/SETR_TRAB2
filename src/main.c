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
#include "../tests/test.h"

int main(void) 
{
    setUp();
	UNITY_BEGIN();

    
    printf("\n---Test Commands---\n");
    RUN_TEST(test_cmdproc_init);
    RUN_TEST(test_command_A);
    RUN_TEST(test_command_P);
    RUN_TEST(test_command_L);
    RUN_TEST(test_command_R);
    RUN_TEST(test_nonexistent_cmd);
    RUN_TEST(test_wrong_values);

// ============================
// 📌 Testes ao comando 'R'
// ============================
    printf("\n========== Testes ao Comando 'R' ==========\n");

    RUN_TEST(test_command_R);                // Comando correto
    RUN_TEST(test_command_R_START_BYTE);     // Falta do símbolo de início
    RUN_TEST(test_command_R_END_BYTE);       // Falta do símbolo de fim

    printf("===========================================\n\n");
 
    printf("\n---Test checksum---\n");
    RUN_TEST(test_wrong_checksum);

    printf("\n---Test convertions---\n");
    RUN_TEST(test_num2char);
    RUN_TEST(test_char2num);

    printf("\n---Test Buffers---\n");
    RUN_TEST(test_rbuff);
    RUN_TEST(test_tbuff);
     
    RUN_TEST(teste_txchar);
    RUN_TEST(teste_rxchar);
    
    
    printf("\n---Test Adding values---\n");
    RUN_TEST(test_addValue);

    
    printf("\n---Test Getting Sensor Data---\n");
    RUN_TEST(test_getsensor);

	tearDown();

	return UNITY_END();
}
