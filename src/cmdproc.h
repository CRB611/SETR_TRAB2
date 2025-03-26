/**
 * \file cmdproc.h
 * \brief This file contains all the structures and functions needed for the C module.
 *
 * This module processes commands received via UART, one character at a time. 
 * The module is part of a smart sensor that reads:
 * <ul>
    <li>Temperature, from -50ºC to 60ºC;</li>
    <li>Relative humidity, from 0% to 100%;</li>
    <li>CO<sub>2</sub>, from 400 to 2000 ppm;</li>
</ul>
 * This file is based on the source file  given to us by professor Paulo Pedreiras. 
 * 
 * \author Simão Ribeiro
 * \author Celina Brito
 * \date 1/4/2025
 * \bug There are no known bugs.
 *
 * \defgroup C Module
 * \brief Functions and structures for the C module. 
 *
 */ 

#ifndef CMD_PROC_H_
#define CMD_PROC_H_

/* Some defines */
/* Other defines should be return codes of the functions */
/* E.g. #define CMD_EMPTY_STRING -1                      */
#define UART_RX_SIZE 20 	///< Maximum size of the RX buffer  
#define UART_TX_SIZE 20 	///< Maximum size of the TX buffer 
#define SOF_SYM '#'	        ///< Start of Frame Symbol
#define EOF_SYM '!'         ///< End of Frame Symbol 

/* Function prototypes */

/* ************************************************************ */
/* Processes the chars in the RX buffer looking for commands 	*/
/* Returns:                                                     */
/*  	 0: if a valid command was found and executed           */
/* 		-1: if empty string or incomplete command found         */
/* 		-2: if an invalid command was found                     */
/* 		-3: if a CS error is detected (command not executed)    */
/* 		-4: if string format is wrong                           */
/* ************************************************************ */
/**
 * \brief Processes the commands 
 * \return 
 */
int cmdProcessor(void);

/* ******************************** */
/* Adds a char to the RX buffer 	*/
/* I.e., the reception of commands 	*/
/* Returns: 				        */
/*  	 0: if success 		        */
/* 		-1: if buffer full	 	    */
/* ******************************** */
int rxChar(unsigned char car);

/* ************************************ */
/* Adds a char to the TX buffer 		*/
/* I.e., the tranmsisison of answers 	*/
/* Returns: 				        	*/
/*  	 0: if success 		        	*/
/* 		-1: if buffer full	 	    	*/
/* ************************************ */
int txChar(unsigned char car);

/* ************************* */
/* Resets the RX buffer		 */  
/* ************************* */
void resetRxBuffer(void);

/* ************************* */
/* Resets the TX buffer		 */  
/* ************************* */
void resetTxBuffer(void);

/* ************************************************ */
/* Return the data that would be sent by the sensor */  
/* ************************************************ */
void getTxBuffer(unsigned char * buf, int * len);

/* ************************************************ */
/* Computes the checksum of a given number of chars */
/* ************************************************ */ 
int calcChecksum(unsigned char * buf, int nbytes);

#endif
