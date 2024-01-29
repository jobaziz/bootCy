/**
 * @file rndis_debug.h
 * @brief RNDIS (Remote Network Driver Interface Specification)
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

#ifndef _RNDIS_DEBUG_H
#define _RNDIS_DEBUG_H

//Dependencies
#include "rndis.h"


/**
 * @brief Value/name pair
 **/

typedef struct
{
   uint32_t value;
   const char_t *name;
} RndisValueName;


//RNDIS related functions
error_t rndisDumpMsg(const RndisMsg *message, size_t length);
error_t rndisDumpPacketMsg(const RndisPacketMsg *message, size_t length);
error_t rndisDumpInitializeMsg(const RndisInitializeMsg *message, size_t length);
error_t rndisDumpHaltMsg(const RndisHaltMsg *message, size_t length);
error_t rndisDumpQueryMsg(const RndisQueryMsg *message, size_t length);
error_t rndisDumpSetMsg(const RndisSetMsg *message, size_t length);
error_t rndisDumpResetMsg(const RndisResetMsg *message, size_t length);
error_t rndisDumpIndicateStatusMsg(const RndisIndicateStatusMsg *message, size_t length);
error_t rndisDumpKeepAliveMsg(const RndisKeepAliveMsg *message, size_t length);

error_t rndisDumpInitializeCmplt(const RndisInitializeCmplt *message, size_t length);
error_t rndisDumpQueryCmplt(const RndisQueryCmplt *message, size_t length);
error_t rndisDumpSetCmplt(const RndisSetCmplt *message, size_t length);
error_t rndisDumpResetCmplt(const RndisResetCmplt *message, size_t length);
error_t rndisDumpKeepAliveCmplt(const RndisKeepAliveCmplt *message, size_t length);

const char_t *rndisFindName(uint32_t value, const RndisValueName *table, size_t size);

#endif
