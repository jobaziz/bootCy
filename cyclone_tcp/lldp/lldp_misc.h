/**
 * @file lldp_misc.h
 * @brief Helper functions for LLDP
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

#ifndef _LLDP_MISC_H
#define _LLDP_MISC_H

//Dependencies
#include "core/net.h"
#include "lldp/lldp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//LLDP multicast address (refer to IEEE Std 802.1AB-2005, section 8.1)
extern const MacAddr LLDP_MULTICAST_ADDR;

//LLDP agent related functions
void lldpTick(LldpAgentContext *context);

void lldpProcessFrame(LldpAgentContext *context);

error_t lldpCheckDataUnit(LldpPortEntry *port, LldpDataUnit *lldpdu);

LldpNeighborEntry *lldpCreateNeighborEntry(LldpAgentContext *context);

LldpNeighborEntry *lldpFindNeighborEntry(LldpAgentContext *context,
   LldpDataUnit *lldpdu);

void lldpDeleteNeighborEntry(LldpNeighborEntry *entry);

bool_t lldpGetLinkState(LldpAgentContext *context, uint_t portIndex);

error_t lldpAcceptMulticastAddr(LldpAgentContext *context);
error_t lldpDropMulticastAddr(LldpAgentContext *context);

void lldpGeneratePortAddr(LldpPortEntry *port);

error_t lldpGetMsapId(LldpDataUnit *lldpdu, LldpMsapId *msapId);
bool_t lldpCompareMsapId(const LldpMsapId *msapId1, const LldpMsapId *msapId2);

void lldpSomethingChangedLocal(LldpAgentContext *context);
void lldpDecrementTimer(uint_t *x);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
