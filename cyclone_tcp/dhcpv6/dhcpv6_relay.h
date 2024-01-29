/**
 * @file dhcpv6_relay.h
 * @brief DHCPv6 relay agent (Dynamic Host Configuration Protocol for IPv6)
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

#ifndef _DHCPV6_RELAY_H
#define _DHCPV6_RELAY_H

//Dependencies
#include "dhcpv6/dhcpv6_common.h"
#include "core/socket.h"

//DHCPv6 relay agent support
#ifndef DHCPV6_RELAY_SUPPORT
   #define DHCPV6_RELAY_SUPPORT ENABLED
#elif (DHCPV6_RELAY_SUPPORT != ENABLED && DHCPV6_RELAY_SUPPORT != DISABLED)
   #error DHCPV6_RELAY_SUPPORT parameter is not valid
#endif

//Stack size required to run the DHCPv6 relay agent
#ifndef DHCPV6_RELAY_STACK_SIZE
   #define DHCPV6_RELAY_STACK_SIZE 500
#elif (DHCPV6_RELAY_STACK_SIZE < 1)
   #error DHCPV6_RELAY_STACK_SIZE parameter is not valid
#endif

//Priority at which the DHCPv6 relay agent should run
#ifndef DHCPV6_RELAY_PRIORITY
   #define DHCPV6_RELAY_PRIORITY OS_TASK_PRIORITY_NORMAL
#endif

//Maximum number of client-facing interfaces
#ifndef DHCPV6_RELAY_MAX_CLIENT_IF
   #define DHCPV6_RELAY_MAX_CLIENT_IF 4
#elif (DHCPV6_RELAY_MAX_CLIENT_IF < 1)
   #error DHCPV6_RELAY_MAX_CLIENT_IF parameter is not valid
#endif

//The amount of overhead added by relay forwarding
#define DHCPV6_RELAY_FORW_OVERHEAD (sizeof(Dhcpv6RelayMessage) + 2 * sizeof(Dhcpv6Option) + sizeof(uint32_t))

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief DHCPv6 relay agent settings
 **/

typedef struct
{
   OsTaskParameters task;                                     ///<Task parameters
   NetInterface *serverInterface;                             ///<Network-facing interface
   NetInterface *clientInterface[DHCPV6_RELAY_MAX_CLIENT_IF]; ///<Client-facing interfaces
   uint_t clientInterfaceCount;                               ///<Number of client-facing interfaces
   Ipv6Addr serverAddress;                                    ///<Address to be used when relaying messages to the server
} Dhcpv6RelaySettings;


/**
 * @brief DHCPv6 relay agent context
 **/

typedef struct
{
   NetInterface *serverInterface;                             ///<Network-facing interface
   NetInterface *clientInterface[DHCPV6_RELAY_MAX_CLIENT_IF]; ///<Client-facing interfaces
   uint_t clientInterfaceCount;                               ///<Number of client-facing interfaces
   Ipv6Addr serverAddress;                                    ///<Address to be used when relaying messages to the server
   Socket *serverSocket;                                      ///<Socket that handles the network-facing interface
   Socket *clientSocket[DHCPV6_RELAY_MAX_CLIENT_IF];          ///<Sockets that handle client-facing interfaces
   SocketEventDesc eventDesc[DHCPV6_RELAY_MAX_CLIENT_IF];     ///<The events the application is interested in
   bool_t running;                                            ///<DHCPv6 relay agent is currently running or not?
   bool_t stopRequest;                                        ///<Stop request
   OsEvent ackEvent;                                          ///<Event object use to acknowledge user requests
   OsEvent event;                                             ///<Event object used to poll the sockets
   OsTaskParameters taskParams;                               ///<Task parameters
   OsTaskId taskId;                                           ///<Task identifier
   uint8_t buffer[DHCPV6_MAX_MSG_SIZE];                       ///<Scratch buffer to store DHCPv6 messages
} Dhcpv6RelayContext;


//DHCPv6 relay agent specific functions
error_t dhcpv6RelayStart(Dhcpv6RelayContext *context, const Dhcpv6RelaySettings *settings);
error_t dhcpv6RelayStop(Dhcpv6RelayContext *context);

error_t dhcpv6RelayJoinMulticastGroup(Dhcpv6RelayContext *context);
error_t dhcpv6RelayLeaveMulticastGroup(Dhcpv6RelayContext *context);

void dhcpv6RelayTask(void *param);

error_t dhcpv6ForwardClientMessage(Dhcpv6RelayContext *context, uint_t index);
error_t dhcpv6ForwardRelayReplyMessage(Dhcpv6RelayContext *context);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
