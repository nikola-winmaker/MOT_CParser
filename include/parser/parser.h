/*
 * parser.h
 *
 *  Created on: Nov 13, 2022
 *      Author: nikola
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

#define USE_STDIO_H
#define USE_STRING_H
//#define DEBUG_ACTIVE

#define RECORD_LENGTH			78u /* less or equal to 78 by standard */
#define BYTES_FOR_TYPE          2u
#define BYTES_FOR_COUNT         (BYTES_FOR_TYPE + 2u)
#define BYTES_FOR_CSUM          2u
#define BYTES_FOR_STREAM_END    2u

// Join A, B to AB
#define JOIN_TWO_NUMBERS(A, B) (A << 4) | B
// Join A, B, C, D to ABCD
#define JOIN_FOUR_NUMBERS(A, B, C, D) (A << 12) | (B << 8) | (C << 4) | D

#ifdef DEBUG_ACTIVE
#ifdef USE_STDIO_H
    #include <stdio.h> /* printf */
    #define console_logger(...) printf(__VA_ARGS__)
#endif
#else
#define console_logger(...) do {} while(0)
#endif

#ifdef USE_STRING_H
#include <string.h> /* memcpy*/
#else
#error "Provide memcpy function!"
#endif

/*! \brief Parser states of incoming stream of byte values
 */
typedef enum parser_fsm_t
{
    GET_RECORD_TYPE, //!< Find which record type we have
    GET_RECORD_COUNT, //!< Find how many bytes left in the record line
    GET_RECORD_ADDRESS, //!< Get address from record line
    GET_RECORD_DATA, //!< Get data from record line
    GET_RECORD_CHECKSUM, //!< Check checksum on data
    GET_STREAM_END //!<  Finished parsing one line of record
} t_parser_fsm;

#endif /* INCLUDE_PARSER_H_ */
