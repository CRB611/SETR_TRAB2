/**
 * \file cmdproc.h
 * \brief This file contains all the structures and functions needed for the C module.
 *
 *  This module processes commands received via UART, one character at a time. 
 * The module is part of a smart sensor that reads:
 * <ul>
    <li>Temperature, from -50ºC to 60ºC;</li>
    <li>Relative humidity, from 0% to 100%;</li>
    <li>CO<sub>2</sub>, from 400 to 2000 ppm;</li>
</ul>
 *  In this file there are functions to initialize the module, to manage the transmission and reception buffers, 
 * to get the information from the sensors and to do the necessary conversions to process the ASCII messages.
 *  This file also has some functions required to emulate the UART.
 *  
 * This file is based on the source file given to us by professor Paulo Pedreiras. 
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
 #define MAX_SIZE 20
 #define UART_RX_SIZE 30 	///< Maximum size of the RX buffer  
 #define UART_TX_SIZE 300 	///< Maximum size of the TX buffer 
 #define SOF_SYM '#'	        ///< Start of Frame Symbol
 #define EOF_SYM '!'         ///< End of Frame Symbol 
 #define EOF_ERROR -10       ///< End of frame error status code
 #define OK 0                ///< Return when a function completes without errors
 #define EMPTY -1            ///< ERROR CODE: Empty string or incomplete command found
 #define INV_COMM -2         ///< ERROR CODE: Invalid command
 #define CS_ERROR -3         ///< ERROR CODE: CS error detected (command not executed)
 #define SOF_ERROR -4        ///< ERROR CODE: The START byte does not exist
 #define FULL_BUFF -5        ///< ERROR CODE: Full buffer
 #define NOT_SENSOR -6       ///< ERROR CODE: Wrong sensor type
 #define CHECKSUM_BAD -7     ///< ERROR CODE: The checksum didnt check out
 #define NOT_EMPTY -8        ///< ERROR CODE: Buffer not empty
 #define END -9              ///< End of function
 #define VALUES_ERROR -11    ///< Value error comand
 #define FATAL_ERROR -12     ///< ERROR CODE: Fatal_Error
 
 /* Function prototypes */
 /**
  * \brief Processes the chars in the RX buffer looking for commands 
  * \return OK if a valid command was found and executed
  * \return EMPTY if a empty string or incomplete command was found
  * \return INV_COMM if a invalid command was found 
  * \return CS_ERROR if a CS error is detected (command not executed) 
  * \return WRONG_FORMAT if the string format is wrong
  */
 int cmdProcessor(void);
 
 /**
  * \brief Adds a char to the reception buffer
  * \return OK if success
  * \return FULL_BUFF if the buffer is full
  */
 int rxChar(unsigned char car);
 
 /**
  * \brief Adds a char to the transmission buffer
  * \return OK if success
  * \return FULL_BUFF if the buffer is full
  */
 int txChar(unsigned char car);
 
  
 /**
  * \brief Resets the reception buffer.
  */
 void resetRxBuffer(void);
 
 /**
  * \brief Resets the transmission buffer.
  */
 void resetTxBuffer(void);
 
 /**
  * \brief Returns the data that is in the transmission buffer.
  * \param buf pointer to the buffer
  * \param len  pointer to the buffer length
  */
 int getTxBuffer(unsigned char * buf, int * len);

 /**
  * \brief Returns the data that is in the reception buffer.
  * \param buf pointer to the buffer
  * \param len  pointer to the buffer length
  */
 int getRxBuffer(unsigned char * buf, int * len);
 
 
 /**
  * \brief Computes the checksum of a given number of chars.
  * \param buf pointer to the buffer
  * \param nbytes number of bytes to check
  */
 int calcChecksum(unsigned char * buf, int nbytes);
 
 /**
  * \brief Checks the length of the TX_BUFF
  * \return Returns FULL_BUFF if the buffer is full and the length otherwise
  *  
  */
 int getTxBufferLen(void);

 /**
  * \brief Checks the length of the RX_BUFF
  * \return Returns FULL_BUFF if the buffer is full and the length otherwise
  *  
  */
 int getRxBufferLen(void);

/**
  * \brief Clears the reception buffer
  * \param len Dimention of the actual buffer
  *  
  */
 void clearRXBuffer(int len);

 /**
  * \brief Converts an integer to a ASCII
  * \param array pointer to where the converted chars will be stored
  * \param num the integer to be converted
  * \param type 't' for temperature, 'h' for humidity, 'c' for c02 or checksum
  */
 void num2char(unsigned char *array, int num, char type);

 /**
  * \brief Converts ASCII to int
  * \param ascii pointer to where the converted chars are stored
  * \param length number of characters in the ascii code
  * \return Returns the int corresponding to the character array
  */
unsigned int char2num(unsigned char ascii [], int length);


 /**
  * \brief Initializes the buffers and all the values arrays
  *  
  */
 int init(void);

 /**
  * \brief Get the temperature array
  * \return The pointer for the temperature array
  */
 void get_temp(int *t);
 
 /**
  * \brief Get the humidity array
  * \return The pointer for the humidity array
  */
 void get_hum(int *h);
 
 /**
  * \brief Get the CO2 array
  * \return The pointer for the CO2 array
  */
 void get_co2(int *c);
 
 /**
  * \brief Get the temp,hum,co2 arrays
  * 
  */
 void get_all( int *t, int *h, int *c);

 /**
  * \brief Get last temp from the array
  */
 int getFirstTemp(void);

 /**
  * \brief Get last hum from the array
  */
 int getFirstHum(void);

 /**
  * \brief Get last co2 from the array
  */
 int getFirstco2(void);
 
 /**
 * \brief Adds a value to the begining of the array by shifting the other elements.
 * \param arr Pointer to the array.
 * \param size Pointer to the actual size of the array.
 * \param value The value to be added.
 */
void addValue(int *array, unsigned int *size, int value);

/**
 * \brief Erases the reception buffer.
 * \param len Current length of the buffer.
 */
void eraseRxBuff(int len);

/**
 * \brief Erases the transmission buffer.
 * \param len Current length of the buffer.
 */
void eraseTxBuff(int len);

void setValues(int t[MAX_SIZE], int h[MAX_SIZE], int c[MAX_SIZE]);
 #endif
 