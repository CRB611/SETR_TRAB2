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

void setUp(void);
 
/**
  * \brief closes the unity testing
  */
void tearDown(void);

/**
  * \brief tests the initialization of the module
  * \par 
  */
void test_cmdproc_init(void);

void test_command_A(void);

void test_command_P(void);

void test_command_L(void);

void test_command_R(void);

#endif