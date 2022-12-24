//
// Created by Nikola  on 24.12.22..
//

#ifndef MOT_CPARSER_PARSER_INTERFACE_H
#define MOT_CPARSER_PARSER_INTERFACE_H

/*! \brief Parser return values
 */
typedef enum parser_ret_t
{
    PARSE_ERROR,
    PARSE_OK,
    PARSER_BUSY
} t_parser_ret;

/*! \brief Record types associated with SREC
 * Invalid record type is -1
 */
typedef enum record_type_t
{
    R_INV   =-1,//!< Invalid record type found
    R_S0    =0, //!< Supported
    R_S1    =1, //!< Supported
    R_S2    =2, //!< Supported
    R_S3    =3, //!< Supported
    R_S4    =4, //!< Not supported
    R_S5    =5, //!< Supported
    R_S6    =6, //!< Supported
    R_S7    =7, //!< Supported
    R_S8    =8, //!< Supported
    R_S9    =9, //!< Supported
} t_record_type;

/*! \brief Record structure associated with SREC
 */
typedef struct record_info_t
{
    t_record_type type;     //!< Type of record S0 - S9
    unsigned short count;   //!< Remaining character pairs in record
    char address_len;       //!< Length of address in bytes
    long address;           //!< Address at which the data field is to be loaded into memory
    char data_len;          //!< Length of data in bytes
    char data[64u];         //!< Actual payload
    unsigned short csum;    //!< Check sum
} t_record_info;

/*! \brief parse_input function prototype for parsing input stream*/
typedef t_parser_ret (*parse_input)(char* data);
/*! \brief get_record function prototype for reading srec data*/
typedef t_parser_ret (*get_record)(t_record_info * record);

/*! \brief Parser interface
 * parse_input parser incoming stream, user needs to provide byte by byte
 * get_record returns pointer to the latest value of the parsed record if parse_input returns
 * PARSE_OK, if returns PARSE_BUSY then still parser needs to finish parsing.
 */
typedef struct parser_t
{
    parse_input parse_input; //!< Blocking parse byte by byte
    get_record  get_record; //! Blocking read
} t_parser;

/*! \brief External linked Parser, information hidden from the user
 */
extern t_parser Parser;

#endif //MOT_CPARSER_PARSER_INTERFACE_H
