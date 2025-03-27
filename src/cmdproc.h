<<<<<<< HEAD
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
#define OK 0                ///< Return when a function completes without errors
#define EMPTY -1            ///< ERROR CODE: Empty string or incomplete command found
#define INV_COMM -2         ///< ERROR CODE: Invalid command
#define CS_ERROR -3         ///< ERROR CODE: CS error detected (command not executed)
#define WRONG_FORMAT -4     ///< ERROR CODE: The string format is wrong
#define FULL_BUFF -5        ///< ERROR CODE: Full buffer
#define NOT_SENSOR -6       ///< ERROR CODE: Wrong sensor type
#define CHECKSUM_BAD -7     ///< ERROR CODE: The checksum didnt check out


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
 * \brief Returns the data that would be sent by the sensor.
 * \param buf
 * \param len
 */
void getTxBuffer(unsigned char * buf, int * len);

/**
 * \brief Computes the checksum of a given number of chars.
 * \param buf
 * \param nbytes
 */
int calcChecksum(unsigned char * buf, int nbytes);

/**
 * \brief Checks the state of the TX_BUFF
 * \return Returns FULL_BUFF if the buffer is full
 *  
 */
int getTxBufferLen(void);

/**
 * \brief Converts an integer to a ASCII
 * \param num the integer to be converted
 * \param type if its 't' the number comes with the +/- sign
 * \return Returns the converted char array
 */
char* num2char(int num, char type);

int getCO2(void);

int getHumidity(void);

int getTemp(void);


#endif
=======
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
/* E.g. #define CMD_EMPTY_STRING -1    
*/
#define MAX_SIZE 20         ///< Maxium size of arrays Temp, Hum , CO2                  
#define UART_RX_SIZE 20 	///< Maximum size of the RX buffer  
#define UART_TX_SIZE 20 	///< Maximum size of the TX buffer 
#define SOF_SYM '#'	        ///< Start of Frame Symbol
#define EOF_SYM '!'         ///< End of Frame Symbol 
#define OK 0                ///< Return when a function completes without errors
#define EMPTY -1            ///< ERROR CODE: Empty string or incomplete command found
#define INV_COMM -2         ///< ERROR CODE: Invalid command
#define CS_ERROR -3         ///< ERROR CODE: CS error detected (command not executed)
#define WRONG_FORMAT -4     ///< ERROR CODE: The string format is wrong
#define FULL_BUFF -5        ///< ERROR CODE: Full buffer
#define NOT_EMPTY -6        ///< ERROR CODE: Buffer not empty


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
 * \brief Returns the data that would be sent by the sensor.
 * \param buf
 * \param len
 */
void getTxBuffer(unsigned char * buf, int * len);

/**
 * \brief Computes the checksum of a given number of chars.
 * \param buf
 * \param nbytes
 */
int calcChecksum(unsigned char * buf, int nbytes);

/**
 * \brief Checks the state of the TX_BUFF
 * \return Returns FULL_BUFF if the buffer is full
 *  
 */
int getTxBufferLen(void);

/**
 * \brief Initialize the buffers and all the values arrays
 *  
 */

int init(void);
/**
 * \brief Get the temperatura array
 * \return The pointer for the temperature array
 */
int* get_temp(void);

/**
 * \brief Get the humidity array
 * \return The pointer for the humidity array
 */
int* get_hum(void);

/**
 * \brief Get the CO2 array
 * \return The pointer for the CO2 array
 */
int* get_co2(void);

/**
 * \brief Get the temp,hum,co2 arrays
 * 
 */
void get_all(void);

/**
 * \brief Get the last temp value from array
 * \return The last temperatura
 * 
 */
int getFirstTemp(void);


/**
 * \brief Get the last humidity value from array
 * \return The last humidity
 */
int getFirstHum(void);


/**
 * \brief Get the last CO2 value from array
 * \return The last CO2
 */
int getFirstco2(void);

#endif
>>>>>>> branch_simao
