/**
 * @file syslog_client.h
 * @brief Syslog client
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

#ifndef _SYSLOG_CLIENT_H
#define _SYSLOG_CLIENT_H

//Dependencies
#include "core/net.h"

//Syslog client support
#ifndef SYSLOG_CLIENT_SUPPORT
   #define SYSLOG_CLIENT_SUPPORT DISABLED
#elif (SYSLOG_CLIENT_SUPPORT != ENABLED && SYSLOG_CLIENT_SUPPORT != DISABLED)
   #error SYSLOG_CLIENT_SUPPORT parameter is not valid
#endif

//Size of internal buffer
#ifndef SYSLOG_CLIENT_BUFFER_SIZE
   #define SYSLOG_CLIENT_BUFFER_SIZE 512
#elif (SYSLOG_CLIENT_BUFFER_SIZE < 128)
   #error SYSLOG_CLIENT_BUFFER_SIZE parameter is not valid
#endif

//Application specific context
#ifndef SYSLOG_CLIENT_PRIVATE_CONTEXT
   #define SYSLOG_CLIENT_PRIVATE_CONTEXT
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Syslog facility values
 **/

typedef enum
{
   SYSLOG_FACILITY_KERNEL       = 0,  ///<Kernel messages
   SYSLOG_FACILITY_USER_LEVEL   = 1,  ///<User-level messages
   SYSLOG_FACILITY_MAIL         = 2,  ///<Mail system
   SYSLOG_FACILITY_SYSTEM       = 3,  ///<System daemons
   SYSLOG_FACILITY_SEC_AUTH     = 4,  ///<Security/authorization messages
   SYSLOG_FACILITY_INTENAL      = 5,  ///<Messages generated internally by syslogd
   SYSLOG_FACILITY_LINE_PRINTER = 6,  ///<Line printer subsystem
   SYSLOG_FACILITY_NETWORK_NEWS = 7,  ///<Network news subsystem
   SYSLOG_FACILITY_UUCP         = 8,  ///<UUCP subsystem
   SYSLOG_FACILITY_CLOCK        = 9,  ///<Clock daemon
   SYSLOG_FACILITY_SEC_AUTH2    = 10, ///<Security/authorization messages
   SYSLOG_FACILITY_FTP          = 11, ///<FTP daemon
   SYSLOG_FACILITY_NTP          = 12, ///<NTP subsystem
   SYSLOG_FACILITY_LOG_AUDIT    = 13, ///<Log audit
   SYSLOG_FACILITY_LOG_ALERT    = 14, ///<Log alert
   SYSLOG_FACILITY_Clock        = 15, ///<Clock daemon
   SYSLOG_FACILITY_LOCAL0       = 16, ///<Local use 0
   SYSLOG_FACILITY_LOCAL1       = 17, ///<Local use 1
   SYSLOG_FACILITY_LOCAL2       = 18, ///<Local use 2
   SYSLOG_FACILITY_LOCAL3       = 19, ///<Local use 3
   SYSLOG_FACILITY_LOCAL4       = 20, ///<Local use 4
   SYSLOG_FACILITY_LOCAL5       = 21, ///<Local use 5
   SYSLOG_FACILITY_LOCAL6       = 22, ///<Local use 6
   SYSLOG_FACILITY_LOCAL7       = 23  ///<Local use 7
} SyslogFacility;


/**
 * @brief Syslog severity values
 **/

typedef enum
{
   SYSLOG_SEVERITY_EMERGENCY = 0, ///<System is unusable
   SYSLOG_SEVERITY_ALERT     = 1, ///<Action must be taken immediately
   SYSLOG_SEVERITY_CRITICAL  = 2, ///<Critical conditions
   SYSLOG_SEVERITY_ERROR     = 3, ///<Error conditions
   SYSLOG_SEVERITY_WARNING   = 4, ///<Warning conditions
   SYSLOG_SEVERITY_NOTICE    = 5, ///<Normal but significant condition
   SYSLOG_SEVERITY_INFO      = 6, ///<Informational messages
   SYSLOG_SEVERITY_DEBUG     = 7  ///<Debug-level messages
} SyslogSeverity;


/**
 * @brief Syslog client context
 **/

typedef struct
{
   OsMutex mutex;                            ///<Mutex preventing simultaneous access to the context
   NetInterface *interface;                  ///<Underlying network interface
   Socket *socket;                           ///<Underlying UDP socket
   char_t buffer[SYSLOG_CLIENT_BUFFER_SIZE]; ///<Internal buffer
   SYSLOG_CLIENT_PRIVATE_CONTEXT             ///<Application specific context
} SyslogClientContext;


//Syslog client related functions
error_t syslogClientInit(SyslogClientContext *context);

error_t syslogClientBindToInterface(SyslogClientContext *context,
   NetInterface *interface);

error_t syslogClientConnect(SyslogClientContext *context,
   const IpAddr *serverIpAddr, uint16_t serverPort);

error_t syslogClientSendMessage(SyslogClientContext *context, uint_t facility,
   uint_t severity, const char_t *message);

error_t syslogClientFormatMessage(SyslogClientContext *context, uint_t facility,
   uint_t severity, const char_t *format, ...);

error_t syslogClientClose(SyslogClientContext *context);

void syslogClientDeinit(SyslogClientContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
