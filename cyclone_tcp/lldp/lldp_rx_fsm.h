/**
 * @file lldp_rx_fsm.h
 * @brief LLDP receive state machine
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

#ifndef _LLDP_RX_FSM_H
#define _LLDP_RX_FSM_H

//Dependencies
#include "core/net.h"
#include "lldp/lldp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief LLDP receive states
 **/

typedef enum
{
   LLDP_STATE_LLDP_WAIT_PORT_OPERATIONAL = 0,
   LLDP_STATE_DELETE_AGED_INFO           = 1,
   LLDP_STATE_RX_LLDP_INITIALIZE         = 2,
   LLDP_STATE_RX_WAIT_FOR_FRAME          = 3,
   LLDP_STATE_RX_FRAME                   = 4,
   LLDP_STATE_DELETE_INFO                = 5,
   LLDP_STATE_UPDATE_INFO                = 6
} LldpRxState;


//LLDP related functions
void lldpInitRxFsm(LldpPortEntry *port);
void lldpRxFsm(LldpPortEntry *port);
void lldpChangeRxState(LldpPortEntry *port, LldpRxState newState);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
