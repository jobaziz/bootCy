/**
 * @file lldp_tx_fsm.h
 * @brief LLDP transmit state machine
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

#ifndef _LLDP_TX_FSM_H
#define _LLDP_TX_FSM_H

//Dependencies
#include "core/net.h"
#include "lldp/lldp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief LLDP transmit states
 **/

typedef enum
{
   LLDP_STATE_TX_LLDP_INITIALIZE = 0,
   LLDP_STATE_TX_IDLE            = 1,
   LLDP_STATE_TX_SHUTDOWN_FRAME  = 2,
   LLDP_STATE_TX_INFO_FRAME      = 3
} LldpTxState;


//LLDP related functions
void lldpInitTxFsm(LldpPortEntry *port);
void lldpTxFsm(LldpPortEntry *port);
void lldpChangeTxState(LldpPortEntry *port, LldpTxState newState);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
