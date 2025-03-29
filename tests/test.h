/**
 * \file test.h
 * \brief This file contains all the functions to make the tests required.
 *
 * In this files are all the functions to test the cmdproc module with UNITY.
 * It includes tests to the initialization, all the commands to process, the conversions required
 * checksums and to buffer related ocurrences.
 * 
 * \author Simão Ribeiro
 * \author Celina Brito
 * \date 1/4/2025
 * \bug There are no known bugs.
 *
 * \defgroup test
 * \brief Functions and structures for testing. 
 * 
 */

#ifndef UNITY_TEST_H_
#define UNITY_TEST_H_

#define CheckError 0

/**
  * \brief Sets up the UNITY testing
  */
void setup(void);
 
/**
  * \brief closes the unity testing
  */
void teardown(void);

/**
  * \brief Tests the initialization of the module
  * 
  */
void test_cmdproc_init(void);

/**
 * \brief Tests the command 'A' using UNITY
 */
void test_command_A(void);

/**
 * \brief Tests the command 'P' using UNITY
 * 
 * This test tests the command 'P' for all the sensors and for a sensor that does not exists
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

/**
 * \brief Tests the occurrence of a nonexistent command being sent
 */
void test_nonexistent_cmd(void);

/**
 * \brief Tests the occurrence of a wrong checksum being sent
 */
void test_wrong_checksum(void);

/**
 * \brief Performs a test for the txchar function using UNITY
 */
void teste_txchar(void);

/**
 * \brief Performs a test for the rxchar function using UNITY
 */
void teste_rxchar(void);

/**
 * \brief Performs a test for the num2char convertion function using UNITY
 */
void test_num2char(void);

/**
 * \brief Performs a test for the char2num convertion function using UNITY
 */
void test_char2num(void);

/**
 * \brief Performs a test for the reception buffer functions using UNITY
 * 
 * To test these functions the buffer length is checked in the ocasion it should be empty, have a determined number of elements or when it is full.
 * The contents on the buffer are also checked.
 */
void test_rbuff(void);

/**
 * \brief Performs a test for the tx buffer functions using UNITY* 
 * 
 * To test these functions the buffer length is checked in the ocasion it should be empty, have a determined number of elements or when it is full.
 * The contents on the buffer are also checked.
 */
void test_tbuff(void);

/**
 * \brief Performs a test for the addValue function using UNITY
 * 
 *  Two tests are made for this function, the first is the value incertion,
 *  and the second is inserting a value with a full list, which "deletes" the last value.
 */
void test_addValue(void);
#endif