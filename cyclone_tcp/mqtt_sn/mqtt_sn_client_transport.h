/**
 * @file mqtt_sn_client_transport.h
 * @brief Transport protocol abstraction layer
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

#ifndef _MQTT_SN_CLIENT_TRANSPORT_H
#define _MQTT_SN_CLIENT_TRANSPORT_H

//Dependencies
#include "core/net.h"
#include "mqtt_sn/mqtt_sn_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT-SN related functions
error_t mqttSnClientOpenConnection(MqttSnClientContext *context, bool_t secure);
error_t mqttSnClientEstablishConnection(MqttSnClientContext *context);
error_t mqttSnClientShutdownConnection(MqttSnClientContext *context);
void mqttSnClientCloseConnection(MqttSnClientContext *context);

error_t mqttSnClientBroadcastDatagram(MqttSnClientContext *context,
   const IpAddr *destIpAddr, uint16_t destPort, const void *data,
   size_t length);

error_t mqttSnClientSendDatagram(MqttSnClientContext *context,
   const void *data, size_t length);

error_t mqttSnClientReceiveDatagram(MqttSnClientContext *context,
   IpAddr *srcIpAddr, uint16_t *srcPort, void *data, size_t size,
   size_t *received, systime_t timeout);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
