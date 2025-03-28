/**
 * \file test.h
 * \brief This file contains all the functions to make the tests required.
 *
 * resumo melhor aqui
 * 
 * \author Simão Ribeiro
 * \author Celina Brito
 * \date 1/4/2025
 * \bug There are no known bugs.
 *
 * \defgroup test
 * \brief Functions and structures for testing. 
 * @{
 */

 /**
  * \brief sets up the unity testing
  */

#ifndef UNITY_TEST_H_
#define UNITY_TEST_H_

#define CheckError 0


void setup(void);
 
/**
  * \brief closes the unity testing
  */
void teardown(void);

/**
  * \brief tests the initialization of the module
  * \par 
  */
void test_cmdproc_init(void);

/**
 * \brief Tests the command 'A' using UNITY
 */
void test_command_A(void);

/**
 * \brief Tests the command 'P' using UNITY
 */
void test_command_P(void);

/**
 * \brief Tests the command 'L' using UNITY
 */
void test_command_L(void);

/**
 * \brief Tests the command 'R' using UNITY
 */
void test_command_R(void);

void test_wrong_checksum(void);

/**
 * \brief Performs a test for the txchar function using UNITY
 */
void teste_txchar(void);

void teste_rxchar(void);

void test_num2char(void);

void test_char2num(void);

void test_rbuff(void);

void test_tbuff(void);
#endif