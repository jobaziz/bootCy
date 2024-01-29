/**
 * @file memory.h
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

#ifndef _MEMORY_H
#define _MEMORY_H

//Dependencies
#include <stdlib.h>
#include <stdint.h>
#include "boot_config.h"
#include "os_port.h"
#include "core/flash.h"
#include "core/cboot_error.h"
#include "error.h"

/*
 *
 * Memory is the abstraction layer between hardware and the library.
 * Inside a memory, there could be one or more slots. Depending on the
 * type of the slot and the way it is implemented within a "memory", it
 * shall expose different APIs and structs.
 *
 */

//Number of memories used
#ifndef NB_MEMORIES
#define NB_MEMORIES 1
#elif (NB_MEMORIES < 1)
#error NB_MEMORIES parameter is not valid
#endif

//Maximum number of slots per memoriess
#ifndef NB_MAX_MEMORY_SLOTS
#define NB_MAX_MEMORY_SLOTS 2
#elif (NB_MAX_MEMORY_SLOTS < 1)
#error NB_MEMORIES parameter is not valid
#endif

//File system layer memory support
#ifndef MEMORIES_FS_SUPPORT
#define MEMORIES_FS_SUPPORT DISABLED
#elif ((MEMORIES_FS_SUPPORT != DISABLED) && ((MEMORIES_FS_SUPPORT != ENABLED))
#error MEMORIES_FS_SUPPORT parameter is not valid
#endif

#if (MEMORIES_FS_SUPPORT == ENABLED)
#include "core/fs.h"
#endif

/**
 * @brief Memory Type definition
 **/
typedef enum
{
    MEMORY_TYPE_FLASH,  // FLASH
#if (MEMORIES_FS_SUPPORT == ENABLED)
    MEMORY_TYPE_FS,     // File system type abstraction
#endif
    MEMORY_TYPE_EEPROM  // EEPROM
} MemoryType;

/**
 * @brief Memory Role definition
 **/
typedef enum
{
    MEMORY_ROLE_PRIMARY,  //Primary memory, most likely internal Flash
    MEMORY_ROLE_SECONDARY //Secondary memory, most likely an external memory
} MemoryRole;

/**
 * @brief Memory status definition
 **/
typedef enum
{
    MEMORY_STATUS_BUSY,
    MEMORY_STATUS_ERROR,
    MEMORY_STATUS_OK
} MemoryStatus;


/**
 * @brief Slot Type definition
 **/
typedef enum
{
    SLOT_TYPE_DIRECT = 1,   // FLASH
#if (MEMORIES_FS_SUPPORT == ENABLED)
    SLOT_TYPE_FILE,     // File system type abstraction
#endif
} SlotType;

/**
 * @brief Slot status definition
 **/
typedef enum
{
    SLOT_STATUS_BUSY,
    SLOT_STATUS_ERROR,
    SLOT_STATUS_OK
} SlotStatus;

/**
 * @brief Slot Type definition
 **/
typedef enum {
    SLOT_CONTENT_NONE           = 0x00, // Each value has its own bit
    SLOT_CONTENT_APP            = 0x01, //
    SLOT_CONTENT_UPDATE         = 0x02, //
    SLOT_CONTENT_BACKUP         = 0x04, //
    SLOT_CONTENT_BINARY         = 0x08, //
    SLOT_CONTENT_DATA           = 0x10, //
    SLOT_CONTENT_CONFIGURATION  = 0x20, //
    SLOT_CONTENT_BOOT           = 0x30  //
} SlotContentType;

/**
 * @brief Slot Type definition
 **/
typedef struct {
    SlotType type;
    uint8_t cType;
    const void *memParent;

    __packed_union {
        __packed_struct {
            uint32_t addr;
            size_t size;
        } /*flash*/;
#if (MEMORIES_FS_SUPPORT == ENABLED)
        __packed_struct {
            const char* path;
            const char* mode;
            FsFileHandler* file;
        } /*filesystem*/;
#endif
    } /*memory*/;

} Slot;


/**
 * @brief Memory Information
 **/

typedef struct
{
    uint32_t addr;
    size_t size;
    uint32_t bank1Addr;
    uint32_t bank2Addr;
    size_t bankSize;
    size_t writeSize;
    uint32_t flags;
}MemoryInfo;


/**
 * @brief Memory Definition
 **/
typedef struct
{
    MemoryType memoryType;
    void *memoryInfo;
    Slot slots[NB_MAX_MEMORY_SLOTS];
    uint8_t nbSlots;
    const void *driver;
    MemoryRole memoryRole;
} Memory;

/**
 * @brief Memory initialization function
 **/
cboot_error_t memoryInit(Memory* memories, size_t nbMemories);


/**
 * @brief Memory de-initialization function
 **/
cboot_error_t memoryDeInit(Memory* memories, size_t nbMemories);


/**
 * @brief Memory get driver information function
 **/
cboot_error_t memoryGetInfo(Memory *memory, MemoryInfo* info);


/**
 * @brief Get Memory Status function
 **/
cboot_error_t memoryGetSlotStatus(Slot *slot, SlotStatus *status);


/**
 * @brief Write Data into Memory function
 **/
cboot_error_t memoryWriteSlot(Slot *slot, uint32_t offset, uint8_t* buffer, size_t length, size_t *written, uint8_t flag);


/**
 * @brief Read Data from Memory function
 **/
cboot_error_t memoryReadSlot(Slot *slot, uint32_t offset, uint8_t* buffer, size_t length);


/**
 * @brief Erase Data from Memory function
 **/
cboot_error_t memoryEraseSlot(Slot *slot, uint32_t offset, size_t length);


/**
 * @brief Make a backup of the internal slot
 **/
cboot_error_t memoryCopySlot(Slot *src, Slot *dst);


/**
 * @brief Memory cleanup function
 **/
cboot_error_t memoryCleanup(Memory* memories, size_t nbMemories);


cboot_error_t memoryGetSlotByCType(Memory* memory, uint8_t slotCType, Slot **slot);


cboot_error_t memoryGetMemoryByRole(Memory* memories, size_t nb_memories, MemoryRole role, Memory **memory);


//Include Extended Memory routines
#include "memory_ex.h"

#endif //_MEMORY_H
