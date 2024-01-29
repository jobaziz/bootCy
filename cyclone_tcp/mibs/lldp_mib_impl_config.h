/**
 * @file lldp_mib_impl_config.h
 * @brief LLDP MIB module implementation (lldpConfiguration subtree)
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

#ifndef _LLDP_MIB_IMPL_CONFIG_H
#define _LLDP_MIB_IMPL_CONFIG_H

//Dependencies
#include "mibs/mib_common.h"
#include "lldp/lldp.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//LLDP MIB related functions
error_t lldpMibSetLldpMessageTxInterval(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpMessageTxInterval(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibSetLldpMessageTxHoldMultiplier(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpMessageTxHoldMultiplier(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibSetLldpReinitDelay(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpReinitDelay(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibSetLldpTxDelay(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpTxDelay(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibSetLldpNotificationInterval(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpNotificationInterval(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibSetLldpPortConfigEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpPortConfigEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibGetNextLldpPortConfigEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

error_t lldpMibSetLldpConfigManAddrEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit);

error_t lldpMibGetLldpConfigManAddrEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen);

error_t lldpMibGetNextLldpConfigManAddrEntry(const MibObject *object, const uint8_t *oid,
   size_t oidLen, uint8_t *nextOid, size_t *nextOidLen);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
