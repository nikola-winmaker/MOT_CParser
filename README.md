# MOT_CParser
Parse MOT file

Motorola S-records\
The following fairly complete description of Motorola S-records was found a number of years ago on the man pages of a UNIX system.\
NAME\
srec - S-record file and record format\
DESCRIPTION\
\
An S-record file consists of a sequence of specially formatted ASCII character strings. An S-record will be less than or equal to 78 bytes in length.
The order of S-records within a file is of no significance and no particular order may be assumed.

The general format of an S-record follows:

+-------------------//------------------//-----------------------+<br/>
| type | count | address  |            data           | checksum |<br/>
+-------------------//------------------//-----------------------+<br/>
type -- A char[2] field. These characters describe the type of record (S0, S1, S2, S3, S5, S7, S8, or S9).
count -- A char[2] field. These characters when paired and interpreted as a hexadecimal value, display the count of remaining character pairs in the record.

address -- A char[4,6, or 8] field. These characters grouped and interpreted as a hexadecimal value, display the address at which the data field is to be loaded into memory. The length of the field depends on the number of bytes necessary to hold the address. A 2-byte address uses 4 characters, a 3-byte address uses 6 characters, and a 4-byte address uses 8 characters.\

data -- A char [0-64] field. These characters when paired and interpreted as hexadecimal values represent the memory loadable data or descriptive information.\

checksum -- A char[2] field. These characters when paired and interpreted as a hexadecimal value display the least significant byte of the ones complement of the sum of the byte values represented by the pairs of characters making up the count, the address, and the data fields.

Each record is terminated with a line feed. If any additional or different record terminator(s) or delay characters are needed during transmission to the target system it is the responsibility of the transmitting program to provide them.

S0 Record. The type of record is 'S0' (0x5330). The address field is unused and will be filled with zeros (0x0000). The header information within the data field is divided into the following subfields.

>> mname is char[20] and is the module name.<br/>
>> ver is char[2] and is the version number.<br/>
>> rev is char[2] and is the revision number.<br/>
>> description is char[0-36] and is a text comment.<br/>

Each of the subfields is composed of ASCII bytes whose associated characters, when paired, represent one byte hexadecimal values in the case of the version and revision numbers, or represent the hexadecimal values of the ASCII characters comprising the module name and description.

S1 Record. The type of record field is 'S1' (0x5331). The address field is intrepreted as a 2-byte address. The data field is composed of memory loadable data.

S2 Record. The type of record field is 'S2' (0x5332). The address field is intrepreted as a 3-byte address. The data field is composed of memory loadable data.

S3 Record. The type of record field is 'S3' (0x5333). The address field is intrepreted as a 4-byte address. The data field is composed of memory loadable data.

S5 Record. The type of record field is 'S5' (0x5335). The address field is intrepreted as a 2-byte value and contains the count of S1, S2, and S3 records previously transmitted. There is no data field.

S7 Record. The type of record field is 'S7' (0x5337). The address field contains the starting execution address and is intrepreted as 4-byte address. There is no data field.

S8 Record. The type of record field is 'S8' (0x5338). The address field contains the starting execution address and is intrepreted as 3-byte address. There is no data field.

S9 Record. The type of record field is 'S9' (0x5339). The address field contains the starting execution address and is intrepreted as 2-byte address. There is no data field.

EXAMPLE<br/>

Shown below is a typical S-record format file.<br/>
S00600004844521B<br/>
S1130000285F245F2212226A000424290008237C2A<br/>
S11300100002000800082629001853812341001813<br/>
S113002041E900084E42234300182342000824A952<br/>
S107003000144ED492<br/>
S5030004F8<br/>
S9030000FC<br/>

The file consists of one S0 record, four S1 records, one S5 record and an S9 record.<br/>

The S0 record is comprised as follows:<br/>

* S0 S-record type S0, indicating it is a header record.<br/>
* 06 Hexadecimal 06 (decimal 6), indicating that six character pairs (or ASCII bytes) follow.<br/>
* 00 00 Four character 2-byte address field, zeroes in this example.<br/>
* 48 44 52 ASCII H, D, and R - "HDR".<br/>
* 1B The checksum.<br/>

The first S1 record is comprised as follows:<br/>
* S1 S-record type S1, indicating it is a data record to be loaded at a 2-byte address.<br/>
* 13 Hexadecimal 13 (decimal 19), indicating that nineteen character pairs, representing a 2 byte address, 16 bytes of binary data, and a 1 byte checksum, follow.<br/>
* 00 00 Four character 2-byte address field; hexidecimal address 0x0000, where the data which follows is to be loaded.<br/>
* 28 5F 24 5F 22 12 22 6A 00 04 24 29 00 08 23 7C Sixteen character pairs representing the actual binary data.<br/>
* 2A The checksum.<br/>

The second and third S1 records each contain 0x13 (19) character pairs and are ended with checksums of 13 and 52, respectively. The fourth S1 record contains 07 character pairs and has a checksum of 92.<br/>

The S5 record is comprised as follows:<br/>

* S5 S-record type S5, indicating it is a count record indicating the number of S1 records<br/>
* 03 Hexadecimal 03 (decimal 3), indicating that three character pairs follow.<br/>
* 00 04 Hexadecimal 0004 (decimal 4), indicating that there are four data records previous to this record.<br/>
* F8 The checksum.<br/>

The S9 record is comprised as follows:<br/>

* S9 S-record type S9, indicating it is a termination record.<br/>
* 03 Hexadecimal 03 (decimal 3), indicating that three character pairs follow.<br/>
* 00 00 The address field, hexadecimal 0 (decimal 0) indicating the starting execution address.<br/>
* FC The checksum.<br/>
