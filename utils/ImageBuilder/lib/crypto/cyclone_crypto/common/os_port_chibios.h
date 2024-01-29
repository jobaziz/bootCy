/**
 * @file os_port_chibios.h
 * @brief RTOS abstraction layer (ChibiOS/RT)
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

#ifndef _OS_PORT_CHIBIOS_H
#define _OS_PORT_CHIBIOS_H

//Dependencies
#include "ch.h"

//Use static or dynamic memory allocation for tasks
#ifndef OS_STATIC_TASK_SUPPORT
   #define OS_STATIC_TASK_SUPPORT DISABLED
#elif (OS_STATIC_TASK_SUPPORT != ENABLED && OS_STATIC_TASK_SUPPORT != DISABLED)
   #error OS_STATIC_TASK_SUPPORT parameter is not valid
#endif

//Invalid task identifier
#define OS_INVALID_TASK_ID NULL
//Self task identifier
#define OS_SELF_TASK_ID NULL

//Task priority (normal)
#ifndef OS_TASK_PRIORITY_NORMAL
   #define OS_TASK_PRIORITY_NORMAL NORMALPRIO
#endif

//Task priority (high)
#ifndef OS_TASK_PRIORITY_HIGH
   #define OS_TASK_PRIORITY_HIGH HIGHPRIO
#endif

//Milliseconds to system ticks
#ifndef OS_MS_TO_SYSTICKS
   #define OS_MS_TO_SYSTICKS(n) (n)
#endif

//System ticks to milliseconds
#ifndef OS_SYSTICKS_TO_MS
   #define OS_SYSTICKS_TO_MS(n) (n)
#endif

//Retrieve 64-bit system time (not implemented)
#ifndef osGetSystemTime64
   #define osGetSystemTime64() osGetSystemTime()
#endif

//Task prologue
#define osEnterTask()
//Task epilogue
#define osExitTask()
//Interrupt service routine prologue
#define osEnterIsr() CH_IRQ_PROLOGUE(); chSysLockFromISR()
//Interrupt service routine epilogue
#define osExitIsr(flag) chSysUnlockFromISR(); CH_IRQ_EPILOGUE()

//Check kernel version
#if (CH_KERNEL_MAJOR < 3)
   #define thread_t Thread
   #define semaphore_t Semaphore
   #define binary_semaphore_t BinarySemaphore
   #define mutex_t Mutex
   #define chThdTerminatedX chThdTerminated
   #define chSemObjectInit chSemInit
   #define chBSemObjectInit chBSemInit
   #define chMtxObjectInit chMtxInit
   #define chVTGetSystemTime chTimeNow
   #define chSysLockFromISR chSysLockFromIsr
   #define chSysUnlockFromISR chSysUnlockFromIsr
   #define THD_WORKING_AREA_SIZE THD_WA_SIZE
   #define MSG_OK RDY_OK
#endif

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief System time
 **/

typedef uint32_t systime_t;


/**
 * @brief Task identifier
 **/

typedef thread_t *OsTaskId;


/**
 * @brief Task control block
 **/

typedef struct
{
   uint64_t dummy;
} OsTaskTcb;


/**
 * @brief Stack data type
 **/

typedef uint32_t OsStackType;


/**
 * @brief Event object
 **/

typedef binary_semaphore_t OsEvent;


/**
 * @brief Semaphore object
 **/

typedef semaphore_t OsSemaphore;


/**
 * @brief Mutex object
 **/

typedef mutex_t OsMutex;


/**
 * @brief Task routine
 **/

typedef void (*OsTaskCode)(void *param);


//Kernel management
void osInitKernel(void);
void osStartKernel(void);

//Task management
OsTaskId osCreateTask(const char_t *name, OsTaskCode taskCode,
   void *param, size_t stackSize, int_t priority);

OsTaskId osCreateStaticTask(const char_t *name, OsTaskCode taskCode,
   void *param, OsTaskTcb *tcb, OsStackType *stack, size_t stackSize,
   int_t priority);

void osDeleteTask(OsTaskId taskId);
void osDelayTask(systime_t delay);
void osSwitchTask(void);
void osSuspendAllTasks(void);
void osResumeAllTasks(void);

//Event management
bool_t osCreateEvent(OsEvent *event);
void osDeleteEvent(OsEvent *event);
void osSetEvent(OsEvent *event);
void osResetEvent(OsEvent *event);
bool_t osWaitForEvent(OsEvent *event, systime_t timeout);
bool_t osSetEventFromIsr(OsEvent *event);

//Semaphore management
bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count);
void osDeleteSemaphore(OsSemaphore *semaphore);
bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout);
void osReleaseSemaphore(OsSemaphore *semaphore);

//Mutex management
bool_t osCreateMutex(OsMutex *mutex);
void osDeleteMutex(OsMutex *mutex);
void osAcquireMutex(OsMutex *mutex);
void osReleaseMutex(OsMutex *mutex);

//System time
systime_t osGetSystemTime(void);

//Memory management
void *osAllocMem(size_t size);
void osFreeMem(void *p);

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
