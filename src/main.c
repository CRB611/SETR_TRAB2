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

    RUN_TEST(test_command_A);
    RUN_TEST(test_command_P);
    RUN_TEST(test_command_L);
    RUN_TEST(test_command_R);

	tearDown();


	return 0;
}
