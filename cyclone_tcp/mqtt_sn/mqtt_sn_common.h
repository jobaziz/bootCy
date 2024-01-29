/**
 * @file mqtt_sn_common.h
 * @brief Definitions common to MQTT-SN client and gateway
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

#ifndef _MQTT_SN_COMMON_H
#define _MQTT_SN_COMMON_H

//Dependencies
#include "core/net.h"

//MQTT-SN port number
#define MQTT_SN_PORT 1883

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//MQTT-SN protocol identifier
#define MQTT_SN_PROTOCOL_ID 0x01
//Reserved topic identifier
#define MQTT_SN_INVALID_TOPIC_ID 0x0000


/**
 * @brief MQTT-SN transport protocols
 **/

typedef enum {
   MQTT_SN_TRANSPORT_PROTOCOL_UDP  = 1, ///<UDP protocol
   MQTT_SN_TRANSPORT_PROTOCOL_DTLS = 2  ///<DTLS protocol
} MqttSnTransportProtocol;


/**
 * @brief Quality of service level
 **/

typedef enum
{
   MQTT_SN_QOS_LEVEL_0       = 0, ///<At most once delivery
   MQTT_SN_QOS_LEVEL_1       = 1, ///<At least once delivery
   MQTT_SN_QOS_LEVEL_2       = 2, ///<Exactly once delivery
   MQTT_SN_QOS_LEVEL_MINUS_1 = 3  ///<No connection setup
} MqttSnQosLevel;


/**
 * @brief Topic ID types
 **/

typedef enum
{
   MQTT_SN_NORMAL_TOPIC_ID     = 0, ///<Normal topic ID
   MQTT_SN_PREDEFINED_TOPIC_ID = 1, ///<Predefined topic ID
   MQTT_SN_NORMAL_TOPIC_NAME   = 0, ///<Normal topic name
   MQTT_SN_SHORT_TOPIC_NAME    = 2  ///<Short topic name
} MqttSnTopicIdType;


/**
 * @brief MQTT-SN message types
 **/

typedef enum
{
   MQTT_SN_MSG_TYPE_ADVERTISE     = 0x00,
   MQTT_SN_MSG_TYPE_SEARCHGW      = 0x01,
   MQTT_SN_MSG_TYPE_GWINFO        = 0x02,
   MQTT_SN_MSG_TYPE_CONNECT       = 0x04,
   MQTT_SN_MSG_TYPE_CONNACK       = 0x05,
   MQTT_SN_MSG_TYPE_WILLTOPICREQ  = 0x06,
   MQTT_SN_MSG_TYPE_WILLTOPIC     = 0x07,
   MQTT_SN_MSG_TYPE_WILLMSGREQ    = 0x08,
   MQTT_SN_MSG_TYPE_WILLMSG       = 0x09,
   MQTT_SN_MSG_TYPE_REGISTER      = 0x0A,
   MQTT_SN_MSG_TYPE_REGACK        = 0x0B,
   MQTT_SN_MSG_TYPE_PUBLISH       = 0x0C,
   MQTT_SN_MSG_TYPE_PUBACK        = 0x0D,
   MQTT_SN_MSG_TYPE_PUBCOMP       = 0x0E,
   MQTT_SN_MSG_TYPE_PUBREC        = 0x0F,
   MQTT_SN_MSG_TYPE_PUBREL        = 0x10,
   MQTT_SN_MSG_TYPE_SUBSCRIBE     = 0x12,
   MQTT_SN_MSG_TYPE_SUBACK        = 0x13,
   MQTT_SN_MSG_TYPE_UNSUBSCRIBE   = 0x14,
   MQTT_SN_MSG_TYPE_UNSUBACK      = 0x15,
   MQTT_SN_MSG_TYPE_PINGREQ       = 0x16,
   MQTT_SN_MSG_TYPE_PINGRESP      = 0x17,
   MQTT_SN_MSG_TYPE_DISCONNECT    = 0x18,
   MQTT_SN_MSG_TYPE_WILLTOPICUPD  = 0x1A,
   MQTT_SN_MSG_TYPE_WILLTOPICRESP = 0x1B,
   MQTT_SN_MSG_TYPE_WILLMSGUPD    = 0x1C,
   MQTT_SN_MSG_TYPE_WILLMSGRESP   = 0x1D,
   MQTT_SN_MSG_TYPE_ENCAPSULATED  = 0xFE
} MqttSnMsgType;


/**
 * @brief MQTT-SN return codes
 **/

typedef enum
{
   MQTT_SN_RETURN_CODE_ACCEPTED                  = 0,
   MQTT_SN_RETURN_CODE_REJECTED_CONGESTION       = 1,
   MQTT_SN_RETURN_CODE_REJECTED_INVALID_TOPIC_ID = 2,
   MQTT_SN_RETURN_CODE_REJECTED_NOT_SUPPORTED    = 3
} MqttSnReturnCode;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma pack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(push, 1)
#endif


/**
 * @brief MQTT-SN flags
 **/

typedef __packed_struct
{
   __packed_union
   {
      uint8_t all;
      __packed_struct
      {
#if defined(_CPU_BIG_ENDIAN) && !defined(__ICCRX__)
         uint8_t dup : 1;
         uint8_t qos : 2;
         uint8_t retain : 1;
         uint8_t will : 1;
         uint8_t cleanSession : 1;
         uint8_t topicIdType : 2;
#else
         uint8_t topicIdType : 2;
         uint8_t cleanSession : 1;
         uint8_t will : 1;
         uint8_t retain : 1;
         uint8_t qos : 2;
         uint8_t dup : 1;
#endif
      };
   };
} MqttSnFlags;


/**
 * @brief Message header
 **/

typedef __packed_struct
{
   uint8_t length;
   uint8_t msgType;
   uint8_t data[];
} MqttSnHeader;


/**
 * @brief Extended message header
 **/

typedef __packed_struct
{
   uint8_t prefix;
   uint16_t length;
   uint8_t msgType;
   uint8_t data[];
} MqttSnExtHeader;


/**
 * @brief ADVERTISE message
 **/

typedef __packed_struct
{
   uint8_t gwId;
   uint16_t duration;
} MqttSnAdvertise;


/**
 * @brief SEARCHGW message
 **/

typedef __packed_struct
{
   uint8_t radius;
} MqttSnSearchGw;


/**
 * @brief GWINFO message
 **/

typedef __packed_struct
{
   uint8_t gwId;
   uint8_t gwAdd[];
} MqttSnGwInfo;


/**
 * @brief CONNECT message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   uint8_t protocolId;
   uint16_t duration;
   char_t clientId[];
} MqttSnConnect;


/**
 * @brief CONNACK message
 **/

typedef __packed_struct
{
   uint8_t returnCode;
} MqttSnConnAck;


/**
 * @brief WILLTOPICREQ message
 **/

typedef void MqttSnWillTopicReq;


/**
 * @brief WILLTOPIC message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   char_t willTopic[];
} MqttSnWillTopic;


/**
 * @brief WILLMSGREQ message
 **/

typedef void MqttSnWillMsgReq;


/**
 * @brief WILLMSG message
 **/

typedef void MqttSnWillMsg;


/**
 * @brief REGISTER message
 **/

typedef __packed_struct
{
   uint16_t topicId;
   uint16_t msgId;
   char_t topicName[];
} MqttSnRegister;


/**
 * @brief REGACK message
 **/

typedef __packed_struct
{
   uint16_t topicId;
   uint16_t msgId;
   uint8_t returnCode;
} MqttSnRegAck;


/**
 * @brief PUBLISH message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   uint16_t topicId;
   uint16_t msgId;
   uint8_t data[];
} MqttSnPublish;


/**
 * @brief PUBACK message
 **/

typedef __packed_struct
{
   uint16_t topicId;
   uint16_t msgId;
   uint8_t returnCode;
} MqttSnPubAck;


/**
 * @brief PUBREC message
 **/

typedef __packed_struct
{
   uint16_t msgId;
} MqttSnPubRec;


/**
 * @brief PUBREL message
 **/

typedef __packed_struct
{
   uint16_t msgId;
} MqttSnPubRel;


/**
 * @brief PUBCOMP message
 **/

typedef __packed_struct
{
   uint16_t msgId;
} MqttSnPubComp;


/**
 * @brief SUBSCRIBE message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   uint16_t msgId;
   char_t topicName[];
} MqttSnSubscribe;


/**
 * @brief SUBACK message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   uint16_t topicId;
   uint16_t msgId;
   uint8_t returnCode;
} MqttSnSubAck;


/**
 * @brief UNSUBSCRIBE message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   uint16_t msgId;
   char_t topicName[];
} MqttSnUnsubscribe;


/**
 * @brief UNSUBACK message
 **/

typedef __packed_struct
{
   uint16_t msgId;
} MqttSnUnsubAck;


/**
 * @brief PINGREQ message
 **/

typedef void MqttSnPingReq;


/**
 * @brief PINGRESP message
 **/

typedef void MqttSnPingResp;


/**
 * @brief DISCONNECT message
 **/

typedef __packed_struct
{
   uint16_t duration;
} MqttSnDisconnect;


/**
 * @brief WILLTOPICUPD message
 **/

typedef __packed_struct
{
   MqttSnFlags flags;
   char_t willTopic[];
} MqttSnWillTopicUpd;


/**
 * @brief WILLTOPICRESP message
 **/

typedef __packed_struct
{
   uint8_t returnCode;
} MqttSnWillTopicResp;


/**
 * @brief WILLMSGUPD message
 **/

typedef void MqttSnWillMsgUpd;


/**
 * @brief WILLMSGRESP message
 **/

typedef __packed_struct
{
   uint8_t returnCode;
} MqttSnWillMsgResp;


//CC-RX, CodeWarrior or Win32 compiler?
#if defined(__CCRX__)
   #pragma unpack
#elif defined(__CWCC__) || defined(_WIN32)
   #pragma pack(pop)
#endif


/**
 * @brief Predefined topic
 **/

typedef struct
{
   const char_t *topicName; ///<Topic name
   uint16_t topicId;        ///<Topic identifier
} MqttSnPredefinedTopic;


//C++ guard
#ifdef __cplusplus
}
#endif

#endif
