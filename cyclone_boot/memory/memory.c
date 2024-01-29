/**
 * @file memory.c
 * @brief CycloneBOOT Memory Layer Abstraction
 *
 * @section License
 *
 * Copyright (C) 2010-2023 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneBOOT Eval.
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

//Switch to the appropriate trace level
#define TRACE_LEVEL CBOOT_TRACE_LEVEL

//Dependencies
#include <stdio.h>
#include <string.h>
#include "os_port.h"
#include "debug.h"
#include "memory.h"

#if !((defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) || \
   defined(__GNUC__) || defined(__CC_ARM) || defined(__IAR_SYSTEMS_ICC__) || \
   defined(__TASKING__) || defined(__CWCC__) || defined(__TI_ARM__))
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#endif

//Private memory-related routines prototypes
cboot_error_t slotsInit(Memory* memory);
bool_t isSlotsOverlap(Slot *slot1, Slot *slot2);
cboot_error_t cleanupSlotHandler(Slot *slot);


/**
 * @brief Memory initialization function
 **/

cboot_error_t memoryInit(Memory* memories, size_t nbMemories)
{
    error_t error;
    uint_t i;
    const void* memoryDriver;
    MemoryType memoryType;
    Memory* memory;

    //Check parameters
    if(memories == NULL || nbMemories == 0)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    // Initialize memories
    for (i = 0; i < nbMemories; i++)
    {
        memory = &memories[i];
        memoryDriver = memory->driver;
        memoryType = memory->memoryType;

        if (memoryType == MEMORY_TYPE_FLASH)
        {
            error = ((const FlashDriver*)memoryDriver)->init();
            if (error != NO_ERROR)
                return CBOOT_ERROR_MEMORY_DRIVER_INIT_FAILED;
        }
#if (MEMORIES_FS_SUPPORT == ENABLED)
        else if (memoryType == MEMORY_TYPE_FS)
        {
            error = ((const FsDriver*)memoryDriver)->init();
            if (error != NO_ERROR)
                return CBOOT_ERROR_MEMORY_DRIVER_INIT_FAILED;
        }
#endif
        else
        {
            return CBOOT_ERROR_UNKNOWN_MEMORY_TYPE;
        }

        // Initialize slots
        slotsInit(memory);

    }

    //Succesful process
    return CBOOT_NO_ERROR;
}


/**
 * @brief Memory de-initialization function
 **/

cboot_error_t memoryDeInit(Memory *memories, size_t nbMemories)
{

    error_t error;
    uint_t i;
    const void* memoryDriver;
    MemoryType memoryType;

    //Check parameters
    if(memories == NULL || nbMemories == 0)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    for (i = 0; i < nbMemories; i++)
    {
        memoryDriver = memories[i].driver;
        memoryType = memories[i].memoryType;

        if (memoryType == MEMORY_TYPE_FLASH)
        {
            error = ((const FlashDriver*)memoryDriver)->deInit();
            if (error != NO_ERROR)
                return CBOOT_ERROR_MEMORY_DRIVER_DEINIT_FAILED;
        }
#if (MEMORIES_FS_SUPPORT == ENABLED)
        else if (memoryType == MEMORY_TYPE_FS)
        {
            error = ((const FsDriver*)memoryDriver)->deInit();
            if (error != NO_ERROR) {
                return CBOOT_ERROR_MEMORY_DRIVER_DEINIT_FAILED;
            }
        }
#endif
        else
        {
            return CBOOT_ERROR_UNKNOWN_MEMORY_TYPE;
        }
    }

    //Successful process
    return CBOOT_NO_ERROR;
}


/**
 * @brief Memory get driver information function
 **/
cboot_error_t memoryGetInfo(Memory *memory, MemoryInfo *info)
{
    error_t error;
    const void* mInfo;

    //Check parameters
    if(memory == NULL || info == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Is memory a flash?
    if(memory->memoryType == MEMORY_TYPE_FLASH)
    {
        //Get flash memory info
        error = ((const FlashDriver*)memory->driver)->getInfo((const FlashInfo**)&mInfo);
        //Is any error?
        if(error)
            return CBOOT_ERROR_MEMORY_DRIVER_GET_INFO_FAILED;

        //Format memory info
        info->addr = ((const FlashInfo*)mInfo)->flashAddr;
        info->size = ((const FlashInfo*)mInfo)->flashSize;
        info->bank1Addr = ((const FlashInfo*)mInfo)->bank1Addr;
        info->bank2Addr = ((const FlashInfo*)mInfo)->bank2Addr;
        info->bankSize = ((const FlashInfo*)mInfo)->bankSize;
        info->writeSize = ((const FlashInfo*)mInfo)->writeSize;
        info->flags = ((const FlashInfo*)mInfo)->flags;
    }
#if (MEMORIES_FS_SUPPORT == ENABLED)
    //Is memory a file system?
    else if(memory->memoryType == MEMORY_TYPE_FS)
    {
        //Get filesystem memory info
        error = ((const FsDriver*)memory->driver)->getInfo((const FsInfo**)&mInfo);
        //Is any error?
        if(error)
            return CBOOT_ERROR_MEMORY_DRIVER_GET_INFO_FAILED;

        //Format memory info
        info->size = ((const FsInfo*)mInfo)->fsSize;
        info->flags = ((const FsInfo*)mInfo)->flags;
    }
#endif
    else
    {
        //Unknown memory type
        return CBOOT_ERROR_UNKNOWN_MEMORY_TYPE;
    }

    //Successful process
    return CBOOT_NO_ERROR;
}

/**
 * @brief Get Memory Status function
 **/

cboot_error_t memoryGetSlotStatus(Slot *slot, SlotStatus *status)
{
    error_t error = NO_ERROR;
    FlashStatus flashStatus = FLASH_STATUS_OK;
#if (MEMORIES_FS_SUPPORT == ENABLED)
    FsStatus fsStatus = FS_STATUS_OK;
#endif

    const void* memoryDriver = ((const Memory*)slot->memParent)->driver;

    if(slot->type == SLOT_TYPE_DIRECT)
    {
        error = ((const FlashDriver*)memoryDriver)->getStatus(&flashStatus);
        if(error)
            return CBOOT_ERROR_MEMORY_DRIVER_GET_STATUS_FAILED;
        *status = (SlotStatus )flashStatus;
    }
#if (MEMORIES_FS_SUPPORT == ENABLED)
    else if (slot->type == SLOT_TYPE_FILE)
    {
        error = ((const FsDriver *)memoryDriver)->getStatus(&fsStatus);
        if(error)
            return CBOOT_ERROR_MEMORY_DRIVER_GET_STATUS_FAILED;
        *status = (SlotStatus )fsStatus;
    }
#endif
    else
    {
        return CBOOT_ERROR_UNKNOWN_SLOT_TYPE;
    }

    //Successful process
    return CBOOT_NO_ERROR;
}


/**
 * @brief Write Data into Memory function
 **/

cboot_error_t memoryWriteSlot(Slot *slot, uint32_t offset, uint8_t* buffer, size_t length, size_t *written, uint8_t flag)
{
    cboot_error_t cboot_error;
    error_t error;
    size_t n;
    static uint8_t tempBuffer[64] = {0};
    static uint8_t *tempBufferPos = tempBuffer;
    static size_t tempBufferLen = 0;
    size_t writeBlockSize;

    Memory *memory;
    MemoryInfo memoryInfo;
    const void* memoryDriver;

    //Check parameters validity
    if(slot == NULL || buffer == NULL || written == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    //Get memory driver
    memory = (Memory*)slot->memParent;
    memoryDriver = memory->driver;

    //Initialize variables
    cboot_error = CBOOT_NO_ERROR;
    error = NO_ERROR;
    *written = 0;

    if(slot->type == SLOT_TYPE_DIRECT)
    {
        //Get memory driver information
        cboot_error = memoryGetInfo(memory, &memoryInfo);
        //Is any error?
        if(cboot_error)
            return cboot_error;

        //Get memory driver write block size
        writeBlockSize = memoryInfo.writeSize;

        //Temporary data flush required?
        if(flag == 2)
        {
            //Reset temporary buffer data
            memset(tempBuffer, 0x00, sizeof(tempBuffer));
            //Reset temporary buffer position
            tempBufferPos = tempBuffer;
            //Reset temporary buffer length
            tempBufferLen = 0;
        }

        //Process incoming data
        while(length > 0)
        {
            //Fill temporary buffer to reach allowed flash memory write block size
            n = MIN(length, writeBlockSize - tempBufferLen);

            //Fill buffer
            memcpy(tempBufferPos, buffer, n);
            //Update temporary buffer position
            tempBufferPos += n;
            //Update temporary buffer length
            tempBufferLen += n;
            //Advance data pointer
            buffer += n;
            //Remaining bytes to process
            length -= n;

            //Enough data to write?
            if(tempBufferLen == writeBlockSize)
            {
                //Write image data into memory
                error = ((const FlashDriver*)memoryDriver)->write(slot->addr + offset, (uint8_t*)tempBuffer, writeBlockSize);
                //Is any error?
                if(error)
                {
                    //Debug message
                    TRACE_ERROR("Failed to write image data into flash memory!\r\n");
                    return CBOOT_ERROR_FAILURE;
                }

                //Update written bytes
                *written += writeBlockSize;

                //Increase offset
                offset += writeBlockSize;

                //Reset temporary buffer position
                tempBufferPos = tempBuffer;
                //Reset temporary buffer length
                tempBufferLen = 0;
            }
        }

        //Temporary data write required?
        if(tempBufferLen != 0 && flag == 1)
        {
            //Complete buffer with padding to reach minimum allowed write block size
            memset(tempBufferPos, 0x00, writeBlockSize - tempBufferLen);

            //Write image data into external flash memory
            error = ((const FlashDriver*)memoryDriver)->write(slot->addr + offset, (uint8_t*)tempBuffer, writeBlockSize);
            //Is any error?
            if(error)
            {
                //Debug message
                TRACE_ERROR("Failed to write image data into memory!\r\n");
                return CBOOT_ERROR_FAILURE;
            }

            //Update written bytes
            *written += writeBlockSize;

            //Increase offset
            offset += writeBlockSize;

            //Reset temporary buffer position
            tempBufferPos = tempBuffer;
            //Reset temporary buffer length
            tempBufferLen = 0;
        }
    }
#if (MEMORIES_FS_SUPPORT == ENABLED)
    else if (slot->type == SLOT_TYPE_FILE)
    {
        error = ((const FsDriver *)memoryDriver)->write(slot->file,offset, buffer,length);
        if(error) {
            cleanupSlotHandler(slot);
            return CBOOT_ERROR_MEMORY_DRIVER_WRITE_FAILED;
        }
    }
#endif
    else
    {
        return CBOOT_ERROR_UNKNOWN_SLOT_TYPE;
    }

    //Successful process
    return CBOOT_NO_ERROR;
}


/**
 * @brief Read Data from Memory function
 **/

cboot_error_t memoryReadSlot(Slot *slot, uint32_t offset, uint8_t* buffer, size_t length)
{
   error_t error = NO_ERROR;

   const void* memoryDriver = ((const Memory*)slot->memParent)->driver;

   if(slot->type == SLOT_TYPE_DIRECT)
   {
      error = ((const FlashDriver*)memoryDriver)->read(slot->addr + offset,buffer,length);
      if(error) {
         cleanupSlotHandler(slot);
         return CBOOT_ERROR_MEMORY_DRIVER_READ_FAILED;
      }
   }
#if (MEMORIES_FS_SUPPORT == ENABLED)
   else if (slot->type == SLOT_TYPE_FILE)
   {
      error = ((const FsDriver *)memoryDriver)->read(slot->file,offset, buffer,length);
      if(error) {
         cleanupSlotHandler(slot);
         return CBOOT_ERROR_MEMORY_DRIVER_READ_FAILED;
      }
   }
#endif
   else
   {
      return CBOOT_ERROR_UNKNOWN_SLOT_TYPE;
   }

   //Successful process
   return CBOOT_NO_ERROR;
}

/**
 * @brief Copy Data from Memory function
 **/
cboot_error_t memoryCopySlot(Slot *src, Slot *dst) {
    return CBOOT_ERROR_FAILURE;
}


/**
 * @brief Erase Data from Memory function
 **/

cboot_error_t memoryEraseSlot(Slot *slot, uint32_t offset, size_t length)
{
    error_t error = NO_ERROR;

    const void* memoryDriver = ((const Memory*)slot->memParent)->driver;

    if(slot->type == SLOT_TYPE_DIRECT)
    {
        error = ((const FlashDriver*)memoryDriver)->erase(slot->addr + offset,length);
        if(error) {
            cleanupSlotHandler(slot);
            return CBOOT_ERROR_MEMORY_DRIVER_ERASE_FAILED;
        }
    }
#if (MEMORIES_FS_SUPPORT == ENABLED)
    else if (slot->type == SLOT_TYPE_FILE)
    {
        error = ((const FsDriver *)memoryDriver)->erase(slot->file,offset,length);
        if(error) {
            cleanupSlotHandler(slot);
            return CBOOT_ERROR_MEMORY_DRIVER_ERASE_FAILED;
        }
    }
#endif
    else
    {
        return CBOOT_ERROR_UNKNOWN_SLOT_TYPE;
    }

    //Successful process
    return CBOOT_NO_ERROR;
}


cboot_error_t memoryCleanup(Memory* memories, size_t nbMemories)
{
    uint8_t i;
    uint8_t j;
    Memory* memory;
    Slot *slot;

    //Check parameters
    if(memories == NULL || nbMemories == 0)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    for (i = 0; i < nbMemories; i++)
    {
        memory = &memories[i];

        //Loop through memory slots
        for(j = 0; j < memory->nbSlots; j++)
        {
            slot = &memory->slots[j];
            cleanupSlotHandler(slot);
        }
    }

    //Successful process
    return CBOOT_NO_ERROR;

}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

bool_t isSlotsOverlap(Slot *slot1, Slot *slot2)
{
    bool_t res;

    res = 0;

    if(slot1 != NULL && slot2 != NULL && slot1->type == slot2->type)
    {
        if(slot1->type == SLOT_TYPE_DIRECT)
        {
            //Are flash slots 1 & 2 overlapping?
            if(((slot1->addr >= slot2->addr) && slot1->addr < (slot2->addr + slot2->size)) ||
                ((slot2->addr >= slot1->addr) && slot2->addr < (slot1->addr + slot1->size)))
                //Slots overlapping
                res = 1;
        }
#if (MEMORIES_FS_SUPPORT == ENABLED)
        else if(slot1->type == SLOT_TYPE_FILE)
        {
            //Are file slot overlapping (they have the same path)?
            if(strcasecmp(slot1->path, slot2->path) == 0)
                //Slots overlapping
                res = 1;
        }
#endif
        else
        {
            //Unknown slot type
            res  = 1;
        }
    }
    else
    {
        //Bad parameters
        res = 1;
    }

    //Return result
    return res;
}

cboot_error_t cleanupSlotHandler(Slot *slot)
{
    error_t error;
    Memory const *memory = (Memory  *)slot->memParent;

#if (MEMORIES_FS_SUPPORT == ENABLED)
    if(slot->type == SLOT_TYPE_FILE) {

        const FsDriver *driver = memory->driver;

        error = driver->close(slot->file);
        if(error)
            return CBOOT_ERROR_MEMORY_DRIVER_CLOSE_FAILED;
    }
#endif

    if(slot->type == SLOT_TYPE_DIRECT) {
        const FlashDriver *driver = memory->driver;
        error = driver->deInit();
        if(error)
            return CBOOT_ERROR_MEMORY_DRIVER_DEINIT_FAILED;
    }

    //Successful process
    return CBOOT_NO_ERROR;
}


/**
 * @brief Search a slot in memory that match the given slot content type.
 * @param memory Pointer to memory in which we are looking for
 * @param slotCType Slot content type we are looking for
 * @param slot Pointer to the slot we are looking for
 * @return cboot_error_t
 **/

cboot_error_t memoryGetSlotByCType(Memory* memory, uint8_t slotCType, Slot **slot)
{
    uint_t i;

    if(memory == NULL || slotCType == 0 || slot == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    *slot = NULL;

    for(i = 0; i < memory->nbSlots; i++)
    {
        if(memory->slots[i].cType == slotCType)
        {
            *slot = &memory->slots[i];
            break;
        }
    }

    if(*slot == NULL)
        return CBOOT_ERROR_FAILURE;
    else
        return CBOOT_NO_ERROR;
}

cboot_error_t slotsInit(Memory* memory) {
    uint_t i;
    const void* memoryDriver;
    Slot *slot;

    //Check parameters
    if(memory == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    if(memory->nbSlots > NB_MAX_MEMORY_SLOTS)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    memoryDriver = memory->driver;

    //Loop through memory slots
    for(i = 0; i < memory->nbSlots; i++)
    {
        slot = &memory->slots[i];

        //Set memory parent for each slot
        slot->memParent = (void*)memory;

        //Is it a direct slot? (flash slot)
        if(slot->type == SLOT_TYPE_DIRECT && memory->memoryRole != MEMORY_ROLE_SECONDARY)
        {
            //Check slot start address matches a sector address
            if(!(((const FlashDriver*)memoryDriver)->isSectorAddr(slot->addr)))
                return CBOOT_ERROR_INVALID_ADDRESS;
        }

        if(i < memory->nbSlots-1)
        {
            //Check slots overlapping
            if(isSlotsOverlap(&memory->slots[0], &memory->slots[i+1]))
                return CBOOT_ERROR_SLOTS_OVERLAP;
        }

#if (MEMORIES_FS_SUPPORT == ENABLED)
        //Is it a file slot? (file system slot)
        if(slot->type == SLOT_TYPE_FILE)
        {
            //Open slot
            slot->file = ((const FsDriver*)memoryDriver)->open(slot->path, 0);
            if(slot->file == NULL)
            {
                return CBOOT_ERROR_MEMORY_DRIVER_OPEN_FAILED;
            }
        }
#endif
    }

    //Successful process
    return CBOOT_NO_ERROR;
}


cboot_error_t memoryGetMemoryByRole(Memory* memories, size_t nb_memories, MemoryRole role, Memory **memory)
{
    uint_t i;

    if(memories == NULL || nb_memories == 0 || memory == NULL)
        return CBOOT_ERROR_INVALID_PARAMETERS;

    *memory = NULL;

    for(i = 0; i < nb_memories; i++)
    {
        if(memories[i].memoryRole == role)
        {
            *memory = &memories[i];
            break;
        }
    }

    if(*memory == NULL)
        return CBOOT_ERROR_FAILURE;
    else
        return CBOOT_NO_ERROR;
}
