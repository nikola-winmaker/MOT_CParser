/*
 * parser.h
 *
 *  Created on: Nov 13, 2022
 *      Author: nikola
 */

#ifndef INCLUDE_PARSER_H_
#define INCLUDE_PARSER_H_

typedef enum record_type_t
{
	R_INV   =-1,
	R_S0    =0,
	R_S1    =1,
	R_S2    =2,
	R_S3    =3,
	R_S5    =5,
	R_S7    =7,
	R_S8    =8,
	R_S9    =9,
} t_record_type;

typedef enum parser_ret_t
{
	PARSE_ERROR,
	PARSE_OK
} t_parser_ret;

typedef enum parser_fsm_t
{
    GET_RECORD_TYPE,
    GET_RECORD_COUNT,
    GET_RECORD_ADDRESS,
    GET_RECORD_DATA,
    GET_RECORD_CHECKSUM
} t_parser_fsm;

typedef struct record_info_t
{
    t_record_type type;
    unsigned short count;
    char address_len;
    long address;
    char data_len;
    char data[64u];
    unsigned short csum;
} t_record_info;



#define RECORD_LENGTH			78u /* less or equal to 78 by standard */
#define BYTES_FOR_TYPE          2u
#define BYTES_FOR_COUNT         (BYTES_FOR_TYPE + 2u)
#define BYTES_FOR_CSUM          2u

t_parser_ret record_parse_sync(char* record, t_record_info *record_info);

#endif /* INCLUDE_PARSER_H_ */
