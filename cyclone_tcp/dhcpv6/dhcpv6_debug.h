/**
 * @file dhcpv6_debug.h
 * @brief Data logging functions for debugging purpose (DHCPv6)
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

#ifndef _DHCPV6_DEBUG_H
#define _DHCPV6_DEBUG_H

//Dependencies
#include "core/net.h"
#include "dhcpv6/dhcpv6_common.h"
#include "debug.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Check current trace level
#if (DHCPV6_TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
   error_t dhcpv6DumpMessage(const void *message, size_t length);
   error_t dhcpv6DumpOptions(const uint8_t *options, size_t length, uint_t level);
   error_t dhcpv6DumpGenericOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpClientIdOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpServerIdOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpIaNaOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpIaTaOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpIaAddrOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpOroOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpPreferenceOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpElapsedTimeOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpRelayMessageOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpAuthOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpServerUnicastOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpStatusCodeOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpRapidCommitOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpUserClassOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpVendorClassOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpVendorSpecificInfoOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpInterfaceIdOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpReconfMessageOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpReconfAcceptOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpDnsServersOption(const Dhcpv6Option *option, uint_t level);
   error_t dhcpv6DumpDomainListOption(const Dhcpv6Option *option, uint_t level);
#else
   #define dhcpv6DumpMessage(message, length)
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
