/**
 * @file tftp_common.h
 * @brief Definitions common to TFTP client and server
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Eval.
 *
 * This software is provided in source form for a short-term evaluation only. The
 * evaluation license expires 90 days after the date you first download the software.
 *
 * If you plan to use this software in a commercial product, you are required to
 * purchase a commercial license from Oryx Embedded SARL.
 *
 * After the 90-day evaluation period, you agree to either purchase a commercial
 * license or delete all copies of this software. If you wish to extend the
 * evaluation period, you must contact sales@oryx-embedded.com.
 *
 * This evaluation software is provided "as is" without warranty of any kind.
 * Technical support is available as an option during the evaluation period.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.3.2
 **/

#ifndef _TFTP_COMMON_H
#define _TFTP_COMMON_H

//Dependencies
#include "core/net.h"

//TFTP port number
#define TFTP_PORT 69

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief TFTP opcodes
 **/

typedef enum
{
   TFTP_OPCODE_RRQ   = 1, ///<Read request
   TFTP_OPCODE_WRQ   = 2, ///<Write request
   TFTP_OPCODE_DATA  = 3, ///<Data
   TFTP_OPCODE_ACK   = 4, ///<Acknowledgment
   TFTP_OPCODE_ERROR = 5, ///<Error
   TFTP_OPCODE_OACK  = 6  ///<Option acknowledgment
} TftpOpcode;


/**
 * @brief TFTP error codes
 **/

typedef enum
{
   TFTP_ERROR_NOT_DEFINED         = 0,
   TFTP_ERROR_FILE_NOT_FOUND      = 1,
   TFTP_ERROR_ACCESS_VIOLATION    = 2,
   TFTP_ERROR_DISK_FULL           = 3,
   TFTP_ERROR_ILLEGAL_OPERATION   = 4,
   TFTP_ERROR_UNKNOWN_TID         = 5,
   TFTP_ERROR_FILE_ALREADY_EXISTS = 6,
   TFTP_ERROR_NO_SUCH_USER        = 7
} TftpErrorCode;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma pack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief Read request packet (RRQ)
 **/

typedef __packed_struct
{
   uint16_t opcode;   //0-1
   char_t filename[]; //2
} TftpRrqPacket;


/**
 * @brief Write request packet (WRQ)
 **/

typedef __packed_struct
{
   uint16_t opcode;   //0-1
   char_t filename[]; //2
} TftpWrqPacket;


/**
 * @brief Data packet (DATA)
 **/

typedef __packed_struct
{
   uint16_t opcode; //0-1
   uint16_t block;  //2-3
   uint8_t data[];  //4
} TftpDataPacket;


/**
 * @brief Acknowledgment packet (ACK)
 **/

typedef __packed_struct
{
   uint16_t opcode; //0-1
   uint16_t block;  //2-3
} TftpAckPacket;


/**
 * @brief Error packet (ERROR)
 **/

typedef __packed_struct
{
   uint16_t opcode;    //0-1
   uint16_t errorCode; //2-3
   char_t errorMsg[];  //4
} TftpErrorPacket;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma unpack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
