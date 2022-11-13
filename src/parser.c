/*
 * main.c
 *
 *  Created on: Nov 13, 2022
 *      Author: nikola
 */

#include <stdio.h>
#include <string.h>
#include "parser.h"

static t_parser_fsm parser_fsm = GET_RECORD_TYPE;

static const int R_ADD_LEN[] = {/*S0*/  2,
                                /*S1*/  2,
                                /*S2*/  3,
                                /*S3*/  4,
                                /*S4*/  -1,
                                /*S5*/  0,
                                /*S6*/  3,
                                /*S7*/  4,
                                /*S8*/  3,
                                /*S9*/  2};

/*Function definition : a2i()*/
int a2i(char* txt)
{
    int sum, digit, i;
    sum = 0;
    for (i = 0; i < strlen(txt); i++) {
        digit = txt[i] - 0x30;
        sum = (sum * 10) + digit;
    }
    return sum;
}

static const long hextable[] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1, 0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1
};

/**
 * @brief convert a hexidecimal string to a signed long
 * will not produce or process negative numbers except
 * to signal error.
 *
 * @param hex without decoration, case insensitive.
 *
 * @return -1 on error, or result (max (sizeof(long)*8)-1 bits)
 */
long hex_to_dec(char *hex) {
    long ret = 0;
    while (*hex && ret >= 0) {
        ret = (ret << 4) | hextable[*hex++];
    }
    return ret;
}

static
t_record_type map_to_record_type(const char* r_data){

	t_record_type r_type;
    //! we need next byte of record after we find th 'S'
    char r_type_next_byte = 0;

    // check endianess, first byte should start with S from standard
    if (r_data[0] == 'S') {
        //! next byte is index 1, we are running on little endian
        r_type_next_byte = r_data[1];
        //printf("Little endian\n");
    }
    else if(r_data[1] == 'S') {
        //! next byte is index 0, we are running on big endian
        r_type_next_byte = r_data[0];
        //printf("Big endian\n");
    }

    //! let's check which type of record we have
    //! Below are all supported types
	switch(r_type_next_byte){
	case 0x30:
		r_type = R_S0;
		break;
	case 0x31:
		r_type = R_S1;
		break;
	case 0x32:
		r_type = R_S2;
		break;
	case 0x33:
		r_type = R_S3;
		break;
	case 0x35:
		r_type = R_S5;
		break;
	case 0x37:
		r_type = R_S7;
		break;
	case 0x38:
		r_type = R_S8;
		break;
	case 0x39:
		r_type = R_S9;
		break;
	default:
		r_type = R_INV;
		break;
	}

	return r_type;
}
static
t_record_type find_record_type(char* record){

	//! Let's assume by default an invalid type
	t_record_type r_type = R_INV;
	//! Check if null pointer received
	if(record != (void*)0){
        //! Find the record type
        r_type = map_to_record_type(record);

	}else{
		// raise some error if needed
		printf("Invalid pointer of to data\n");
	}

	return r_type;
}

static
unsigned short find_record_count(char* record){

    //! Let's assume count is 0
    unsigned short count = 0;
    //! Check if null pointer received
    if(record != (void*)0){
        count = hex_to_dec(record);
    }else{
        // raise some error if needed
        printf("Invalid pointer of to data\n");
    }

    return count;
}

static
long find_record_adress(char* record){

    //! Let's assume address is 0
    long address = 0;
    //! Check if null pointer received
    if(record != (void*)0){
        address = hex_to_dec(record);
    }else{
        // raise some error if needed
        printf("Invalid pointer of to data\n");
    }

    return address;
}



t_parser_ret record_parse_sync(char* record, t_record_info *record_info){
    //! Let's assume the worst case
	t_parser_ret ret = PARSE_ERROR;
    //! local record for processing
    static t_record_info local_record = {0};
    //! Keep cound of received bytes
    static unsigned char byte_cnt = 0u;
    //! Var for bytes to copy ro record info
    unsigned int copy_bytes = 0u;
    //! Make some initial value larger than inital byte count size of record
    static unsigned char record_end_cnt = RECORD_LENGTH + 1u;
    //! Buffer to keep one record
    static char record_buffer[RECORD_LENGTH] = {0};

    //! buffer the incoming bytes
    record_buffer[byte_cnt] = *record;
    //! keep track of received bytes
    byte_cnt++;

    switch (parser_fsm) {
        case GET_RECORD_TYPE:
            //! we need 2 bytes of data for type
            if(byte_cnt >= BYTES_FOR_TYPE){
                //! Find the record type
                //! These characters describe the type of record (S0, S1, S2, S3, S5, S7, S8, or S9).
                local_record.type = find_record_type(record_buffer);
                if(local_record.type != R_INV){
                    //! Next state
                    parser_fsm = GET_RECORD_COUNT;
                    //! log the type
                    record_info->type  = local_record.type;
                }else{
                    //stay in this state
                    ret = PARSE_ERROR;
                    byte_cnt = 0;
                }
            }
            break;
        case GET_RECORD_COUNT:
            //! we need 2 bytes of data for type
            if(byte_cnt >= BYTES_FOR_COUNT) {

                //! Find the record count
                //! These characters when paired and interpreted as a hexadecimal value,
                //! display the count of remaining character pairs in the record.
                local_record.count = find_record_count(record_buffer + BYTES_FOR_TYPE);
                if((local_record.count !=0) && (local_record.count <= RECORD_LENGTH)) {
                    //! log the type
                    record_info->count = local_record.count;
                    //! character pairs
                    record_end_cnt = byte_cnt + local_record.count * 2u/*2 characters*/;
                    //! Next state
                    parser_fsm = GET_RECORD_ADDRESS;
                }else{
                    //stay in this state
                    ret = PARSE_ERROR;
                    byte_cnt = 0;
                }
            }
            break;
        case GET_RECORD_ADDRESS:
            //! Check how many bytes we need depending on record type
            local_record.address_len = R_ADD_LEN[local_record.type] * 2u/*2 characters*/;
            record_info->address_len = R_ADD_LEN[local_record.type] * 2u/*2 characters*/;
            //! we need 2 bytes of data for type
            if(byte_cnt >= BYTES_FOR_COUNT + local_record.address_len) {
                //! Find the address
                //! These characters grouped and interpreted as a hexadecimal value,
                //! display the address at which the data field is to be loaded into memory.
                //! The length of the field depends on the number of bytes necessary to hold the address.
                //! A 2-byte address uses 4 characters, a 3-byte address uses 6 characters, and a 4-byte address uses 8 characters.
                local_record.address = find_record_adress(record_buffer + BYTES_FOR_COUNT);
                record_info->address = local_record.address;

                //! Next state
                parser_fsm = GET_RECORD_DATA;

                //! can't check the error as address could be anything
            }
            break;
        case GET_RECORD_DATA:

            //! The number of bytes of data contained in this record is
            //! "Byte Count Field" minus 3 (that is, 2 bytes for "16-bit Address Field" and 1 byte for "Checksum Field").
            //S006 0000 484452 1B
            //6-3 = 3
            local_record.data_len = (local_record.count - (R_ADD_LEN[local_record.type] + 1u/*csum*/)) * 2/*2 characters*/;
            //! Wait for bytes to be collected
            if(byte_cnt >= (BYTES_FOR_COUNT + local_record.address_len + local_record.data_len)) {
                copy_bytes = BYTES_FOR_COUNT + local_record.address_len;
                memcpy(local_record.data, record_buffer + copy_bytes, local_record.data_len);
                memcpy(record_info->data, record_buffer + copy_bytes, local_record.data_len);

                //! Next state
                parser_fsm = GET_RECORD_CHECKSUM;
            }
            break;
        case GET_RECORD_CHECKSUM:

            if(byte_cnt >= (BYTES_FOR_COUNT + local_record.address_len + local_record.data_len + BYTES_FOR_CSUM)) {
                //! These characters when paired and interpreted as a hexadecimal value display the
                //! least significant byte of the ones complement of the sum of the byte values
                //! represented by the pairs of characters making up the count, the address, and the data fields.
                copy_bytes = BYTES_FOR_COUNT + local_record.address_len + local_record.data_len;
                //! Convert until '/0'
                local_record.csum = hex_to_dec(record_buffer + copy_bytes);
                record_info->csum = hex_to_dec(record_buffer + copy_bytes);
                //! Next state
                //parser_fsm = GET_RECORD_TYPE;
            }
            break;
        default:
            ret = PARSE_ERROR;
            byte_cnt = 0;
            parser_fsm = GET_RECORD_TYPE;
            break;
    }

    //! reset parser internal states
    if(byte_cnt >= record_end_cnt){
        parser_fsm = GET_RECORD_TYPE;
//        printf("Type is s%d\n", record_info->type);
//        printf("Count is %d\n", record_info->count);
//        printf("Address len is %d\n", local_record.address_len);
//        printf("Address  is %ld\n", local_record.address);

        byte_cnt = 0;
        memset(record_buffer, 0u, RECORD_LENGTH);
        memset(&local_record, 0u, sizeof(local_record));

    }

    return ret;
}
