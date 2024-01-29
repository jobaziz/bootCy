/**
 * @file smi_driver.h
 * @brief Serial management interface driver
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
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
 * @version 3.0.0
 **/

//Dependencies
#include "core/net.h"

#ifndef _SMI_DRIVER_H
#define _SMI_DRIVER_H

//SMI driver
extern const SmiDriver smiDriver;

//SMI related functions
error_t smiInit(void);

void smiWritePhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr, uint16_t data);

uint16_t smiReadPhyReg(uint8_t opcode, uint8_t phyAddr,
   uint8_t regAddr);

void smiWriteData(uint32_t data, uint_t length);
uint32_t smiReadData(uint_t length);

#endif
