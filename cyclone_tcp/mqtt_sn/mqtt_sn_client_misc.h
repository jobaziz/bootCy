/**
 * @file mqtt_sn_client_misc.h
 * @brief Helper functions for MQTT-SN client
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

#ifndef _MQTT_SN_CLIENT_MISC_H
#define _MQTT_SN_CLIENT_MISC_H

//Dependencies
#include "core/net.h"
#include "mqtt_sn/mqtt_sn_client.h"

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT-SN client related functions
error_t mqttSnClientProcessEvents(MqttSnClientContext *context,
   systime_t timeout);

MqttSnReturnCode mqttSnDeliverPublishMessage(MqttSnClientContext *context,
   MqttSnFlags flags, uint16_t topicId, const uint8_t *data, size_t dataLen);

error_t mqttSnClientAddTopic(MqttSnClientContext *context,
   const char_t *topicName, uint16_t topicId);

error_t mqttSnClientDeleteTopic(MqttSnClientContext *context,
   const char_t *topicName);

const char_t *mqttSnClientFindTopicId(MqttSnClientContext *context,
   uint16_t topicId);

uint16_t mqttSnClientFindTopicName(MqttSnClientContext *context,
   const char_t *topicName);

const char_t *mqttSnClientFindPredefTopicId(MqttSnClientContext *context,
   uint16_t topicId);

uint16_t mqttSnClientFindPredefTopicName(MqttSnClientContext *context,
   const char_t *topicName);

uint16_t mqttSnClientGenerateMessageId(MqttSnClientContext *context);

error_t mqttSnClientStoreMessageId(MqttSnClientContext *context,
   uint16_t msgId);

error_t mqttSnClientDiscardMessageId(MqttSnClientContext *context,
   uint16_t msgId);

bool_t mqttSnClientIsDuplicateMessageId(MqttSnClientContext *context,
   uint16_t msgId);

bool_t mqttSnClientIsShortTopicName(const char_t *topicName);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
