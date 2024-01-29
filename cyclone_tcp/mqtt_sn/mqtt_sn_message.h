/**
 * @file mqtt_sn_message.h
 * @brief MQTT-SN message formatting and parsing
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

#ifndef _MQTT_SN_MESSAGE_H
#define _MQTT_SN_MESSAGE_H

//Dependencies
#include "core/net.h"
#include "mqtt_sn/mqtt_sn_common.h"

//Maximum size of MQTT-SN messages
#ifndef MQTT_SN_MAX_MSG_SIZE
   #define MQTT_SN_MAX_MSG_SIZE 1024
#elif (MQTT_SN_MAX_MSG_SIZE < 16)
   #error MQTT_SN_MAX_MSG_SIZE parameter is not valid
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief MQTT-SN message
 **/

typedef struct
{
   uint8_t buffer[MQTT_SN_MAX_MSG_SIZE + 1];
   size_t length;
   size_t pos;
} MqttSnMessage;


//MQTT-SN related functions
error_t mqttSnFormatHeader(MqttSnMessage *message, MqttSnMsgType type,
   size_t length);

error_t mqttSnFormatSearchGw(MqttSnMessage *message,
   uint8_t radius);

error_t mqttSnFormatConnect(MqttSnMessage *message, MqttSnFlags flags,
   uint16_t duration, const char_t *clientId);

error_t mqttSnFormatWillTopic(MqttSnMessage *message, MqttSnFlags flags,
   const char_t *topicName);

error_t mqttSnFormatWillMsg(MqttSnMessage *message, const void *data,
   size_t dataLen);

error_t mqttSnFormatRegister(MqttSnMessage *message,
   uint16_t msgId, uint16_t topicId, const char_t *topicName);

error_t mqttSnFormatRegAck(MqttSnMessage *message, uint16_t msgId,
   uint16_t topicId, MqttSnReturnCode returnCode);

error_t mqttSnFormatPublish(MqttSnMessage *message, MqttSnFlags flags,
   uint16_t msgId, uint16_t topicId, const char_t *topicName,
   const uint8_t *data, size_t dataLen);

error_t mqttSnFormatPubAck(MqttSnMessage *message, uint16_t msgId,
   uint16_t topicId, MqttSnReturnCode returnCode);

error_t mqttSnFormatPubRec(MqttSnMessage *message, uint16_t msgId);
error_t mqttSnFormatPubRel(MqttSnMessage *message, uint16_t msgId);
error_t mqttSnFormatPubComp(MqttSnMessage *message, uint16_t msgId);

error_t mqttSnFormatSubscribe(MqttSnMessage *message, MqttSnFlags flags,
   uint16_t msgId, uint16_t topicId, const char_t *topicName);

error_t mqttSnFormatUnsubscribe(MqttSnMessage *message, MqttSnFlags flags,
   uint16_t msgId, uint16_t topicId, const char_t *topicName);

error_t mqttSnFormatPingReq(MqttSnMessage *message, const char_t *clientId);
error_t mqttSnFormatPingResp(MqttSnMessage *message);

error_t mqttSnFormatDisconnect(MqttSnMessage *message,
   uint16_t duration);

error_t mqttSnFormatWillTopicUpd(MqttSnMessage *message, MqttSnFlags flags,
   const char_t *topicName);

error_t mqttSnFormatWillMsgUpd(MqttSnMessage *message, const void *data,
   size_t dataLen);

error_t mqttSnParseHeader(MqttSnMessage *message, MqttSnMsgType *type);

error_t mqttSnParseGwInfo(const MqttSnMessage *message, uint8_t *gwId,
   const uint8_t **gwAdd, size_t *gwAddLen);

error_t mqttSnParseConnAck(const MqttSnMessage *message,
   MqttSnReturnCode *returnCode);

error_t mqttSnParseWillTopicReq(const MqttSnMessage *message);
error_t mqttSnParseWillMsgReq(const MqttSnMessage *message);

error_t mqttSnParseRegister(const MqttSnMessage *message, uint16_t *msgId,
   uint16_t *topicId, const char_t **topicName);

error_t mqttSnParseRegAck(const MqttSnMessage *message, uint16_t *msgId,
   uint16_t *topicId, MqttSnReturnCode *returnCode);

error_t mqttSnParsePublish(const MqttSnMessage *message, MqttSnFlags *flags,
   uint16_t *msgId, uint16_t *topicId, const uint8_t **data, size_t *dataLen);

error_t mqttSnParsePubAck(const MqttSnMessage *message, uint16_t *msgId,
   uint16_t *topicId, MqttSnReturnCode *returnCode);

error_t mqttSnParsePubRec(const MqttSnMessage *message, uint16_t *msgId);
error_t mqttSnParsePubRel(const MqttSnMessage *message, uint16_t *msgId);
error_t mqttSnParsePubComp(const MqttSnMessage *message, uint16_t *msgId);

error_t mqttSnParseSubAck(const MqttSnMessage *message, MqttSnFlags *flags,
   uint16_t *msgId, uint16_t *topicId, MqttSnReturnCode *returnCode);

error_t mqttSnParseUnsubAck(const MqttSnMessage *message, uint16_t *msgId);

error_t mqttSnParsePingReq(const MqttSnMessage *message,
   const char_t **clientId);

error_t mqttSnParsePingResp(const MqttSnMessage *message);

error_t mqttSnParseDisconnect(const MqttSnMessage *message,
   uint16_t *duration);

error_t mqttSnParseWillTopicResp(const MqttSnMessage *message,
   MqttSnReturnCode *returnCode);

error_t mqttSnParseWillMsgResp(const MqttSnMessage *message,
   MqttSnReturnCode *returnCode);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
