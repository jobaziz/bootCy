/**
 * @file lldp_fsm.c
 * @brief LLDP state machine
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

//Switch to the appropriate trace level
#define TRACE_LEVEL LLDP_TRACE_LEVEL

//Dependencies
#include "core/net.h"
#include "lldp/lldp.h"
#include "lldp/lldp_fsm.h"
#include "lldp/lldp_rx_fsm.h"
#include "lldp/lldp_tx_fsm.h"
#include "lldp/lldp_debug.h"
#include "debug.h"

//Check TCP/IP stack configuration
#if (LLDP_SUPPORT == ENABLED)


/**
 * @brief LLDP state machine initialization
 * @param[in] context Pointer to the LLDP agent context
 **/

void lldpInitFsm(LldpAgentContext *context)
{
   uint_t i;
   LldpPortEntry *port;

   //Loop through the ports
   for(i = 0; i < context->numPorts; i++)
   {
      //Point to the current port
      port = &context->ports[i];

#if (LLDP_RX_MODE_SUPPORT == ENABLED)
      //Initialize LLDP receive state machine
      lldpInitRxFsm(port);
#endif

#if (LLDP_TX_MODE_SUPPORT == ENABLED)
      //Initialize LLDP transmit state machine
      lldpInitTxFsm(port);
#endif
   }

   //Update LLDP state machines
   lldpFsm(context);
}


/**
 * @brief LLDP state machine implementation
 * @param[in] context Pointer to the LLDP agent context
 **/

void lldpFsm(LldpAgentContext *context)
{
   uint_t i;

   //The operation of the LLDP protocol can be represented with two simple
   //state machines
   do
   {
      //Clear the busy flag
      context->busy = FALSE;

#if (LLDP_RX_MODE_SUPPORT == ENABLED)
      //Loop through the ports
      for(i = 0; i < context->numPorts; i++)
      {
         //Update the LLDP receive state machine
         lldpRxFsm(&context->ports[i]);
      }
#endif

#if (LLDP_TX_MODE_SUPPORT == ENABLED)
      //Check whether the RSTP state machine is idle
      if(!context->busy)
      {
         //Loop through the ports
         for(i = 0; i < context->numPorts; i++)
         {
            //Update the LLDP transmit state machine
            lldpTxFsm(&context->ports[i]);
         }
      }
#endif

      //Transition conditions are evaluated continuously as long as the LLDP
      //state machine is busy
   } while(context->busy);
}


/**
 * @brief LLDP state machine error handler
 * @param[in] context Pointer to the LLDP agent context
 **/

void lldpFsmError(LldpAgentContext *context)
{
   //Debug message
   TRACE_ERROR("LLDP state machine error!\r\n");
}

#endif
