/**
  ******************************************************************************
  * @file    ymodem.c
  * @author  MCD Application Team
  * @brief   This file provides all the software functions related to the ymodem
  *          protocol.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

//Switch to the appropriate trace level
#define TRACE_LEVEL TRACE_LEVEL_INFO

//Dependencies
#include "common.h"
#include "ymodem.h"
#include "string.h"
#include "debug.h"
#include "error.h"
#include "core/cboot_error.h"

#define CRC16_F       /* activate the CRC16 integrity */

//Filename buffer
uint8_t aFileName[FILE_NAME_LENGTH];

/* @note ATTENTION - please keep this variable 32bit aligned */
uint8_t aPacketData[PACKET_1K_SIZE + PACKET_DATA_INDEX + PACKET_TRAILER_SIZE];

//Extern Y-Modem Uart handler
extern UART_HandleTypeDef UartHandle;

//Y-Modem private related routines
static HAL_StatusTypeDef ReceivePacket(uint8_t *p_data, uint32_t *p_length, uint32_t timeout);
uint16_t UpdateCRC16(uint16_t crc_in, uint8_t byte);
uint16_t Cal_CRC16(const uint8_t* p_data, uint32_t size);
uint8_t CalcChecksum(const uint8_t *p_data, uint32_t size);
cboot_error_t Ymodem_Receive_UserCallback(uint8_t *data, size_t length);


/**
 * @brief Receive a packet from sender
 * @param[out] data
 * @param[out] length
 *     0: end of transmission
 *     2: abort by sender
 *    >0: packet length
 * @param[in]  timeout
 * @return HAL_OK: normally return
 *         HAL_BUSY: abort by user
 **/

static HAL_StatusTypeDef ReceivePacket(uint8_t *p_data, uint32_t *p_length, uint32_t timeout)
{
  uint32_t crc;
  uint32_t packet_size = 0;
  HAL_StatusTypeDef status;
  uint8_t char1;

  *p_length = 0;
  status = HAL_UART_Receive(&UartHandle, &char1, 1, timeout);

  if (status == HAL_OK)
  {
    switch (char1)
    {
      case SOH:
        packet_size = PACKET_SIZE;
        break;
      case STX:
        packet_size = PACKET_1K_SIZE;
        break;
      case EOT:
        break;
      case CA:
        if ((HAL_UART_Receive(&UartHandle, &char1, 1, timeout) == HAL_OK) && (char1 == CA))
        {
          packet_size = 2;
        }
        else
        {
          status = HAL_ERROR;
        }
        break;
      case ABORT1:
      case ABORT2:
        status = HAL_BUSY;
        break;
      default:
        status = HAL_ERROR;
        break;
    }
    *p_data = char1;

    if (packet_size >= PACKET_SIZE )
    {
      status = HAL_UART_Receive(&UartHandle, &p_data[PACKET_NUMBER_INDEX], packet_size + PACKET_OVERHEAD_SIZE, timeout);

      /* Simple packet sanity check */
      if (status == HAL_OK )
      {
        if (p_data[PACKET_NUMBER_INDEX] != ((p_data[PACKET_CNUMBER_INDEX]) ^ NEGATIVE_BYTE))
        {
          packet_size = 0;
          status = HAL_ERROR;
        }
        else
        {
          /* Check packet CRC */
          crc = p_data[ packet_size + PACKET_DATA_INDEX ] << 8;
          crc += p_data[ packet_size + PACKET_DATA_INDEX + 1 ];
          if (Cal_CRC16(&p_data[PACKET_DATA_INDEX], packet_size) != crc )
          {
            packet_size = 0;
            status = HAL_ERROR;
          }
        }
      }
      else
      {
        packet_size = 0;
      }
    }
  }
  *p_length = packet_size;
  return status;
}


/**
 * @brief Update CRC16 for input byte
 * @param[in] crc_in input value
 * @param[in] input byte
 * @return None
 **/

uint16_t UpdateCRC16(uint16_t crc_in, uint8_t byte)
{
  uint32_t crc = crc_in;
  uint32_t in = byte | 0x100;

  do
  {
    crc <<= 1;
    in <<= 1;
    if(in & 0x100)
      ++crc;
    if(crc & 0x10000)
      crc ^= 0x1021;
  }

  while(!(in & 0x10000));

  return crc & 0xffffu;
}


/**
 * @brief Cal CRC16 for YModem Packet
 * @param[in] data
 * @param[in] length
 * @return None
 **/

uint16_t Cal_CRC16(const uint8_t* p_data, uint32_t size)
{
  uint32_t crc = 0;
  const uint8_t* dataEnd = p_data+size;

  while(p_data < dataEnd)
    crc = UpdateCRC16(crc, *p_data++);

  crc = UpdateCRC16(crc, 0);
  crc = UpdateCRC16(crc, 0);

  return crc&0xffffu;
}


/**
 * @brief Calculate Check sum for YModem Packet
 * @param[in] p_data Pointer to input data
 * @param[in] size length of input data
 * @return uint8_t checksum value
 **/

uint8_t CalcChecksum(const uint8_t *p_data, uint32_t size)
{
  uint32_t sum = 0;
  const uint8_t *p_data_end = p_data + size;

  while (p_data < p_data_end )
  {
    sum += *p_data++;
  }

  return (sum & 0xffu);
}


/**
 * @brief Receive a file using the ymodem protocol with CRC16.
 * @param[out] p_size The size of the file.
 * @return COM_StatusTypeDef result of reception/programming
 **/

COM_StatusTypeDef Ymodem_Receive ( uint32_t *p_size )
{
  uint32_t i, packet_length, session_done = 0, file_done, errors = 0, session_begin = 0, packets_received = 0, bytes_received = 0;
  uint32_t filesize;
  uint8_t *file_ptr, *data_ptr;
  uint8_t file_size[FILE_SIZE_LENGTH];
  COM_StatusTypeDef result = COM_OK;

  while ((session_done == 0) && (result == COM_OK))
  {
    packets_received = 0;
    file_done = 0;
    while ((file_done == 0) && (result == COM_OK))
    {
      switch (ReceivePacket(aPacketData, &packet_length, DOWNLOAD_TIMEOUT))
      {
        case HAL_OK:
          errors = 0;
          switch (packet_length)
          {
            case 2:
              /* Abort by sender */
              Serial_PutByte(ACK);
              result = COM_ABORT;
              break;
            case 0:
              /* End of transmission */
              Serial_PutByte(ACK);
              file_done = 1;
              break;
            default:
              /* Normal packet */
              if (aPacketData[PACKET_NUMBER_INDEX] != (0xFFU & packets_received))
              {
                Serial_PutByte(NAK);
              }
              else
              {
                if (packets_received == 0)
                {
                  /* File name packet */
                  if (aPacketData[PACKET_DATA_INDEX] != 0)
                  {
                    /* File name extraction */
                    i = 0;
                    file_ptr = aPacketData + PACKET_DATA_INDEX;
                    while ( (*file_ptr != 0) && (i < FILE_NAME_LENGTH))
                    {
                      aFileName[i++] = *file_ptr++;
                    }

                    /* File size extraction */
                    aFileName[i++] = '\0';
                    i = 0;
                    file_ptr ++;
                    while ( (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH))
                    {
                      file_size[i++] = *file_ptr++;
                    }
                    file_size[i++] = '\0';
                    Str2Int(file_size, &filesize);
                    *p_size = filesize;

                    //Debug message
                    TRACE_INFO("\r\n");
                    TRACE_INFO("Receiving file \"%s\" (%" PRIu32 " bytes)...\r\n", aFileName, filesize);
                    TRACE_INFO("\r\n");

                    Serial_PutByte(ACK);
                    Serial_PutByte(CRC16);
                  }
                  /* File header packet is empty, end session */
                  else
                  {
                    Serial_PutByte(ACK);
                    file_done = 1;
                    session_done = 1;
                    break;
                  }
                }
                else /* Data packet */
                {
                   /* Data extraction */
                   data_ptr = (uint8_t*)&aPacketData[PACKET_DATA_INDEX];

                   //Debug message
                   TRACE_DEBUG("Packet received (block: %d, %d bytes):\r\n", packets_received, packet_length);

                   //Check if remaining bytes is less than 1024?
                   if((filesize - bytes_received) < 1024)
                   {
                      //Discard '1A' fill data block
                      while(data_ptr[packet_length-1] == '\x1A')
                      {
                         packet_length--;
                      }
                   }

                   //Update received data bytes
                   bytes_received += packet_length;

                   //Debug message
                   TRACE_DEBUG("- Data size : %d\r\n", packet_length);
                   TRACE_DEBUG("- Recv bytes : %d/%d\r\n", bytes_received, filesize);
                   TRACE_DEBUG("- Remaining bytes : %d\r\n", filesize - bytes_received);

                   /* Process received file data here */
                   if(Ymodem_Receive_UserCallback(data_ptr, packet_length) == CBOOT_NO_ERROR)
                   {
                      Serial_PutByte(ACK);
                   }
                  else /* An error occurred while writing to Flash memory */
                  {
                    /* End session */
                    Serial_PutByte(CA);
                    Serial_PutByte(CA);
                    result = COM_DATA;
                  }
                }
                packets_received ++;
                session_begin = 1;
              }
              break;
          }
          break;
        case HAL_BUSY: /* Abort actually */
          Serial_PutByte(CA);
          Serial_PutByte(CA);
          result = COM_ABORT;
          break;
        default:
          if (session_begin > 0)
          {
            errors ++;
          }
          if (errors > MAX_ERRORS)
          {
            /* Abort communication */
            Serial_PutByte(CA);
            Serial_PutByte(CA);
          }
          else
          {
            Serial_PutByte(CRC16); /* Ask for a packet */
          }
          break;
      }
    }
  }
  return result;
}


/**
 * Y-Modem Receive Data User callback (WEAK)
 * This routine is weak and does nothing per default.
 * It MUST be redefined to be able to process received data.
 * @param[in] data Pointer to the received data
 * @param[in] length Lenght of the received data
 * @return Error code
 **/

__weak cboot_error_t Ymodem_Receive_UserCallback(uint8_t *data, size_t length)
{
   return CBOOT_NO_ERROR;
}

/*******************(C)COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
