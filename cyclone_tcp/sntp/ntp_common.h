/**
 * @file ntp_common.h
 * @brief Definitions common to NTP client and server
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

#ifndef _NTP_COMMON_H
#define _NTP_COMMON_H

//Dependencies
#include "core/net.h"

//NTP port number
#define NTP_PORT 123
//Maximum size of NTP messages
#define NTP_MAX_MSG_SIZE 68
//Difference between NTP and Unix time scales
#define NTP_UNIX_EPOCH 2208988800U

//Kiss code definition
#define NTP_KISS_CODE(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Leap indicator
 **/

typedef enum
{
   NTP_LI_NO_WARNING           = 0,
   NTP_LI_LAST_MIN_HAS_61_SECS = 1,
   NTP_LI_LAST_MIN_HAS_59_SECS = 2,
   NTP_LI_ALARM_CONDITION      = 3
} NtpLeapIndicator;


/**
 * @brief NTP version numbers
 **/

typedef enum
{
   NTP_VERSION_1 = 1,
   NTP_VERSION_2 = 2,
   NTP_VERSION_3 = 3,
   NTP_VERSION_4 = 4
} NtpVersion;


/**
 * @brief Protocol modes
 **/

typedef enum
{
   NTP_MODE_SYMMETRIC_ACTIVE  = 1,
   NTP_MODE_SYMMETRIC_PASSIVE = 2,
   NTP_MODE_CLIENT            = 3,
   NTP_MODE_SERVER            = 4,
   NTP_MODE_BROADCAST         = 5
} NtpMode;


/**
 * @brief Kiss codes
 *
 * The kiss codes can provide useful information for an intelligent client.
 * These codes are encoded in four-character ASCII strings left justified
 * and zero filled
 *
 **/

typedef enum
{
   NTP_KISS_CODE_ACST = NTP_KISS_CODE('A', 'C', 'S', 'T'), ///<The association belongs to a anycast server
   NTP_KISS_CODE_AUTH = NTP_KISS_CODE('A', 'U', 'T', 'H'), ///<Server authentication failed
   NTP_KISS_CODE_AUTO = NTP_KISS_CODE('A', 'U', 'T', 'O'), ///<Autokey sequence failed
   NTP_KISS_CODE_BCST = NTP_KISS_CODE('B', 'C', 'S', 'T'), ///<The association belongs to a broadcast server
   NTP_KISS_CODE_CRYP = NTP_KISS_CODE('C', 'R', 'Y', 'P'), ///<Cryptographic authentication or identification failed
   NTP_KISS_CODE_DENY = NTP_KISS_CODE('D', 'E', 'N', 'Y'), ///<Access denied by remote server
   NTP_KISS_CODE_DROP = NTP_KISS_CODE('D', 'R', 'O', 'P'), ///<Lost peer in symmetric mode
   NTP_KISS_CODE_RSTR = NTP_KISS_CODE('R', 'S', 'T', 'R'), ///<Access denied due to local policy
   NTP_KISS_CODE_INIT = NTP_KISS_CODE('I', 'N', 'I', 'T'), ///<The association has not yet synchronized for the first time
   NTP_KISS_CODE_MCST = NTP_KISS_CODE('M', 'C', 'S', 'T'), ///<The association belongs to a manycast server
   NTP_KISS_CODE_NKEY = NTP_KISS_CODE('N', 'K', 'E', 'Y'), ///<No key found
   NTP_KISS_CODE_RATE = NTP_KISS_CODE('R', 'A', 'T', 'E'), ///<Rate exceeded
   NTP_KISS_CODE_RMOT = NTP_KISS_CODE('R', 'M', 'O', 'T'), ///<Somebody is tinkering with the association from a remote host running ntpdc
   NTP_KISS_CODE_STEP = NTP_KISS_CODE('S', 'T', 'E', 'P')  ///<A step change in system time has occurred
} NtpKissCode;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma pack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief NTP timestamp representation
 **/

typedef __packed_struct
{
   uint32_t seconds;
   uint32_t fraction;
} NtpTimestamp;


/**
 * @brief NTP packet header
 **/

typedef __packed_struct
{
#if defined(_CPU_BIG_ENDIAN) && !defined(__ICCRX__)
   uint8_t li : 2;                  //0
   uint8_t vn : 3;
   uint8_t mode : 3;
#else
   uint8_t mode : 3;                //0
   uint8_t vn : 3;
   uint8_t li : 2;
#endif
   uint8_t stratum;                 //1
   uint8_t poll;                    //2
   int8_t precision;                //3
   uint32_t rootDelay;              //4-7
   uint32_t rootDispersion;         //8-11
   uint32_t referenceId;            //12-15
   NtpTimestamp referenceTimestamp; //16-23
   NtpTimestamp originateTimestamp; //24-31
   NtpTimestamp receiveTimestamp;   //32-39
   NtpTimestamp transmitTimestamp;  //40-47
} NtpHeader;


/**
 * @brief NTP authenticator
 **/

typedef __packed_struct
{
   uint32_t keyId;            //0-3
   uint8_t messageDigest[16]; //4-19
} NtpAuthenticator;


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
