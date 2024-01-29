/**
 * @file usbd_desc.h
 * @brief USB descriptors
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

#ifndef _USBD_DESC_H
#define _USBD_DESC_H

//Dependencies
#include "os_port.h"

//Endpoint identifiers
#define USB_EP0 0
#define USB_EP1 1
#define USB_EP2 2
#define USB_EP3 3

//Data transfer direction
#define USB_DIR_MASK 0x80
#define USB_DIR_OUT  0x00
#define USB_DIR_IN   0x80

//Endpoint maximum packet size
#define USB_EP0_MAX_PACKET_SIZE 64
#define USB_EP1_MAX_PACKET_SIZE 64
#define USB_EP2_MAX_PACKET_SIZE 64
#define USB_EP3_MAX_PACKET_SIZE 64

//bmAttributes field
#define USB_SELF_POWERED        0xC0
#define USB_BUS_POWERED         0x80
#define USB_REMOTE_WAKEUP       0xA0
#define USB_NO_REMOTE_WAKEUP    0x80

//Endpoint types
#define USB_ENDPOINT_TYPE_MASK        0x03
#define USB_ENDPOINT_TYPE_CONTROL     0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS 0x01
#define USB_ENDPOINT_TYPE_BULK        0x02
#define USB_ENDPOINT_TYPE_INTERRUPT   0x03

//Device class
#define USB_DEVICE_CLASS_CDC    0x02
//Device subclass
#define USB_DEVICE_SUBCLASS_CDC 0x00
//Device protocol
#define USB_DEVICE_PROTOCOL_CDC 0x00

//CDC communication interface class
#define CDC_INTERFACE_CLASS_COMMUNICATION      0x02
#define CDC_INTERFACE_CLASS_RNDIS              0xEF
//CDC communication interface subclass
#define CDC_INTERFACE_SUBCLASS_ACM             0x02
#define CDC_INTERFACE_SUBCLASS_RNDIS           0x04
//CDC communication interface protocol
#define CDC_INTERFACE_PROTOCOL_RNDIS           0x01
#define CDC_INTERFACE_PROTOCOL_VENDOR_SPECIFIC 0xFF

//CDC data interface class
#define CDC_INTERFACE_CLASS_DATA    0x0A
//CDC data interface subclass
#define CDC_INTERFACE_SUBCLASS_DATA 0x00
//CDC data interface protocol
#define CDC_INTERFACE_PROTOCOL_DATA 0x00

//CDC descriptor types
#define CDC_CS_INTERFACE 0x24
#define CDC_CS_ENDPOINT  0x25

//CDC descriptor subtypes
#define CDC_HEADER_DESC_SUBTYPE          0x00
#define CDC_CALL_MANAGEMENT_DESC_SUBTYPE 0x01
#define CDC_ACM_DESC_SUBTYPE             0x02
#define CDC_UNION_DESC_SUBTYPE           0x06


/**
 * @brief Device descriptor
 **/

typedef __packed_struct
{
   uint8_t bLength;
   uint8_t bDescriptorType;
   uint16_t bcdUsb;
   uint8_t bDeviceClass;
   uint8_t bDeviceSubClass;
   uint8_t bDeviceProtocol;
   uint8_t bMaxPacketSize0;
   uint16_t idVendor;
   uint16_t idProduct;
   uint16_t bcdDevice;
   uint8_t iManufacturer;
   uint8_t iProduct;
   uint8_t iSerialNumber;
   uint8_t bNumConfigurations;
} UsbDeviceDescriptor;


/**
 * @brief Configuration descriptor
 **/

typedef __packed_struct
{
   uint8_t bLength;
   uint8_t bDescriptorType;
   uint16_t wTotalLength;
   uint8_t bNumInterfaces;
   uint8_t bConfigurationValue;
   uint8_t iConfiguration;
   uint8_t bmAttributes;
   uint8_t bMaxPower;
} UsbConfigDescriptor;


/**
 * @brief Interface descriptor
 **/

typedef __packed_struct
{
   uint8_t bLength;
   uint8_t bDescriptorType;
   uint8_t bInterfaceNumber;
   uint8_t bAlternateSetting;
   uint8_t bNumEndpoints;
   uint8_t bInterfaceClass;
   uint8_t bInterfaceSubClass;
   uint8_t bInterfaceProtocol;
   uint8_t iInterface;
} UsbInterfaceDescriptor;


/**
 * @brief Endpoint descriptor
 **/

typedef __packed_struct
{
   uint8_t bLength;
   uint8_t bDescriptorType;
   uint8_t bEndpointAddress;
   uint8_t bmAttributes;
   uint16_t wMaxPacketSize;
   uint8_t bInterval;
} UsbEndpointDescriptor;


/**
 * @brief String descriptor
 **/

typedef __packed_struct
{
   uint8_t bLength;
   uint8_t bDescriptorType;
   uint16_t bString[];
} UsbStringDescriptor;


/**
 * @brief CDC header functional descriptor
 **/

typedef __packed_struct
{
   uint8_t bFunctionLength;
   uint8_t bDescriptorType;
   uint8_t bDescriptorSubtype;
   uint16_t bcdCdc;
} CdcHeaderDescriptor;


/**
 * @brief CDC call management functional descriptor
 **/

typedef __packed_struct
{
   uint8_t bFunctionLength;
   uint8_t bDescriptorType;
   uint8_t bDescriptorSubtype;
   uint8_t bmCapabilities;
   uint8_t bDataInterface;
} CdcCallManagementDescriptor;


/**
 * @brief CDC abstract control management functional descriptor
 **/

typedef __packed_struct
{
   uint8_t bFunctionLength;
   uint8_t bDescriptorType;
   uint8_t bDescriptorSubtype;
   uint8_t bmCapabilities;
} CdcAcmDescriptor;


/**
 * @brief CDC union functional descriptor
 **/

typedef __packed_struct
{
   uint8_t bFunctionLength;
   uint8_t bDescriptorType;
   uint8_t bDescriptorSubtype;
   uint8_t bMasterInterface;
   uint8_t bSlaveInterface0;
} CdcUnionDescriptor;


/**
 * @brief Configuration descriptors
 **/

typedef __packed_struct
{
   //Standard configuration descriptor
   UsbConfigDescriptor configDescriptor;
   //Communication class interface descriptor
   UsbInterfaceDescriptor communicationInterfaceDescriptor;
   //CDC header functional descriptor
   CdcHeaderDescriptor cdcHeaderDescriptor;
   //CDC call management functional descriptor
   CdcCallManagementDescriptor cdcCallManagementDescriptor;
   //CDC abstract control management functional descriptor
   CdcAcmDescriptor cdcAcmDescriptor;
   //CDC union functional descriptor
   CdcUnionDescriptor cdcUnionDescriptor;
   //Notification endpoint descriptor
   UsbEndpointDescriptor notificationEndpointDescriptor;
   //Data class interface descriptor
   UsbInterfaceDescriptor dataInterfaceDescriptor;
   //Data OUT endpoint descriptor
   UsbEndpointDescriptor dataOutEndpointDescriptor;
   //Data IN endpoint descriptor
   UsbEndpointDescriptor dataInEndpointDescriptor;
} UsbConfigDescriptors;


//Global variables
extern USBD_DescriptorsTypeDef usbdRndisDescriptors;
extern const UsbDeviceDescriptor usbdDeviceDescriptor;
extern const UsbConfigDescriptors usbdConfigDescriptors;

//USB related functions
uint8_t *usbdGetDeviceDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *usbdGetLangIdStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *usbdGetManufacturerStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *usbdGetProductStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *usbdGetSerialStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *usbdGetConfigStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);
uint8_t *usbdGetInterfaceStrDescriptor(USBD_SpeedTypeDef speed, uint16_t *length);

#endif
