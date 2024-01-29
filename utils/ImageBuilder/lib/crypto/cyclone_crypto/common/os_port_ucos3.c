/**
 * @file os_port_ucos3.c
 * @brief RTOS abstraction layer (Micrium uC/OS-III)
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

//Switch to the appropriate trace level
#define TRACE_LEVEL TRACE_LEVEL_OFF

//Dependencies
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os_port.h"
#include "os_port_ucos3.h"
#include "debug.h"


/**
 * @brief Kernel initialization
 **/

void osInitKernel(void)
{
   OS_ERR err;

   //Scheduler initialization
   OSInit(&err);
}


/**
 * @brief Start kernel
 **/

void osStartKernel(void)
{
   OS_ERR err;

   //Start the scheduler
   OSStart(&err);
}


/**
 * @brief Create a task with statically allocated memory
 * @param[in] name A name identifying the task
 * @param[in] taskCode Pointer to the task entry function
 * @param[in] param A pointer to a variable to be passed to the task
 * @param[in] tcb Pointer to the task control block
 * @param[in] stack Pointer to the stack
 * @param[in] stackSize The initial size of the stack, in words
 * @param[in] priority The priority at which the task should run
 * @return Task identifier referencing the newly created task
 **/

OsTaskId osCreateStaticTask(const char_t *name, OsTaskCode taskCode,
   void *param, OsTaskTcb *tcb, OsStackType *stack, size_t stackSize,
   int_t priority)
{
   OS_ERR err;
   CPU_STK stackLimit;

   //The watermark limit is used to monitor and ensure that the stack does
   //not overflow
   stackLimit = stackSize / 10;

   //Create a new task
   OSTaskCreate(tcb, (CPU_CHAR *) name, taskCode, param, priority,
      stack, stackLimit, stackSize, 0, 1,
      NULL, OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR, &err);

   //Check whether the task was successfully created
   if(err == OS_ERR_NONE)
   {
      return (OsTaskId) tcb;
   }
   else
   {
      return OS_INVALID_TASK_ID;
   }
}


/**
 * @brief Delete a task
 * @param[in] taskId Task identifier referencing the task to be deleted
 **/

void osDeleteTask(OsTaskId taskId)
{
   OS_ERR err;

   //Delete the specified task
   OSTaskDel((OS_TCB *) taskId, &err);
}


/**
 * @brief Delay routine
 * @param[in] delay Amount of time for which the calling task should block
 **/

void osDelayTask(systime_t delay)
{
   OS_ERR err;

   //Delay the task for the specified duration
   OSTimeDly(OS_MS_TO_SYSTICKS(delay), OS_OPT_TIME_DLY, &err);
}


/**
 * @brief Yield control to the next task
 **/

void osSwitchTask(void)
{
   //Force a context switch
   OSSched();
}


/**
 * @brief Suspend scheduler activity
 **/

void osSuspendAllTasks(void)
{
   OS_ERR err;

   //Make sure the operating system is running
   if(OSRunning == OS_STATE_OS_RUNNING)
   {
      //Suspend scheduler activity
      OSSchedLock(&err);
   }
}


/**
 * @brief Resume scheduler activity
 **/

void osResumeAllTasks(void)
{
   OS_ERR err;

   //Make sure the operating system is running
   if(OSRunning == OS_STATE_OS_RUNNING)
   {
      //Resume scheduler activity
      OSSchedUnlock(&err);
   }
}


/**
 * @brief Create an event object
 * @param[in] event Pointer to the event object
 * @return The function returns TRUE if the event object was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateEvent(OsEvent *event)
{
   OS_ERR err;

   //Create an event flag group
   OSFlagCreate(event, "EVENT", 0, &err);

   //Check whether the event flag group was successfully created
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Delete an event object
 * @param[in] event Pointer to the event object
 **/

void osDeleteEvent(OsEvent *event)
{
   OS_ERR err;

   //Make sure the operating system is running
   if(OSRunning == OS_STATE_OS_RUNNING)
   {
      //Properly dispose the event object
      OSFlagDel(event, OS_OPT_DEL_ALWAYS, &err);
   }
}


/**
 * @brief Set the specified event object to the signaled state
 * @param[in] event Pointer to the event object
 **/

void osSetEvent(OsEvent *event)
{
   OS_ERR err;

   //Set the specified event to the signaled state
   OSFlagPost(event, 1, OS_OPT_POST_FLAG_SET, &err);
}


/**
 * @brief Set the specified event object to the nonsignaled state
 * @param[in] event Pointer to the event object
 **/

void osResetEvent(OsEvent *event)
{
   OS_ERR err;

   //Force the specified event to the nonsignaled state
   OSFlagPost(event, 1, OS_OPT_POST_FLAG_CLR, &err);
}


/**
 * @brief Wait until the specified event is in the signaled state
 * @param[in] event Pointer to the event object
 * @param[in] timeout Timeout interval
 * @return The function returns TRUE if the state of the specified object is
 *   signaled. FALSE is returned if the timeout interval elapsed
 **/

bool_t osWaitForEvent(OsEvent *event, systime_t timeout)
{
   OS_ERR err;

   //Wait until the specified event is in the signaled state or the timeout
   //interval elapses
   if(timeout == 0)
   {
      //Non-blocking call
      OSFlagPend(event, 1, 0, OS_OPT_PEND_FLAG_SET_ANY |
         OS_OPT_PEND_FLAG_CONSUME | OS_OPT_PEND_NON_BLOCKING, NULL, &err);
   }
   else if(timeout == INFINITE_DELAY)
   {
      //Infinite timeout period
      OSFlagPend(event, 1, 0, OS_OPT_PEND_FLAG_SET_ANY |
         OS_OPT_PEND_FLAG_CONSUME | OS_OPT_PEND_BLOCKING, NULL, &err);
   }
   else
   {
      //Wait until the specified event becomes set
      OSFlagPend(event, 1, OS_MS_TO_SYSTICKS(timeout), OS_OPT_PEND_FLAG_SET_ANY |
         OS_OPT_PEND_FLAG_CONSUME | OS_OPT_PEND_BLOCKING, NULL, &err);
   }

   //Check whether the specified event is set
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Set an event object to the signaled state from an interrupt service routine
 * @param[in] event Pointer to the event object
 * @return TRUE if setting the event to signaled state caused a task to unblock
 *   and the unblocked task has a priority higher than the currently running task
 **/

bool_t osSetEventFromIsr(OsEvent *event)
{
   OS_ERR err;

   //Set the specified event to the signaled state
   OSFlagPost(event, 1, OS_OPT_POST_FLAG_SET, &err);

   //The return value is not relevant
   return FALSE;
}


/**
 * @brief Create a semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 * @param[in] count The maximum count for the semaphore object. This value
 *   must be greater than zero
 * @return The function returns TRUE if the semaphore was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateSemaphore(OsSemaphore *semaphore, uint_t count)
{
   OS_ERR err;

   //Create a semaphore
   OSSemCreate(semaphore, "SEMAPHORE", count, &err);

   //Check whether the semaphore was successfully created
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Delete a semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 **/

void osDeleteSemaphore(OsSemaphore *semaphore)
{
   OS_ERR err;

   //Make sure the operating system is running
   if(OSRunning == OS_STATE_OS_RUNNING)
   {
      //Properly dispose the specified semaphore
      OSSemDel(semaphore, OS_OPT_DEL_ALWAYS, &err);
   }
}


/**
 * @brief Wait for the specified semaphore to be available
 * @param[in] semaphore Pointer to the semaphore object
 * @param[in] timeout Timeout interval
 * @return The function returns TRUE if the semaphore is available. FALSE is
 *   returned if the timeout interval elapsed
 **/

bool_t osWaitForSemaphore(OsSemaphore *semaphore, systime_t timeout)
{
   OS_ERR err;

   //Wait until the semaphore is available or the timeout interval elapses
   if(timeout == 0)
   {
      //Non-blocking call
      OSSemPend(semaphore, 0, OS_OPT_PEND_NON_BLOCKING, NULL, &err);
   }
   else if(timeout == INFINITE_DELAY)
   {
      //Infinite timeout period
      OSSemPend(semaphore, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
   }
   else
   {
      //Wait until the specified semaphore becomes available
      OSSemPend(semaphore, OS_MS_TO_SYSTICKS(timeout),
         OS_OPT_PEND_BLOCKING, NULL, &err);
   }

   //Check whether the specified semaphore is available
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Release the specified semaphore object
 * @param[in] semaphore Pointer to the semaphore object
 **/

void osReleaseSemaphore(OsSemaphore *semaphore)
{
   OS_ERR err;

   //Release the semaphore
   OSSemPost(semaphore, OS_OPT_POST_1, &err);
}


/**
 * @brief Create a mutex object
 * @param[in] mutex Pointer to the mutex object
 * @return The function returns TRUE if the mutex was successfully
 *   created. Otherwise, FALSE is returned
 **/

bool_t osCreateMutex(OsMutex *mutex)
{
   OS_ERR err;

   //Create a mutex
   OSMutexCreate(mutex, "MUTEX", &err);

   //Check whether the mutex was successfully created
   if(err == OS_ERR_NONE)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}


/**
 * @brief Delete a mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osDeleteMutex(OsMutex *mutex)
{
   OS_ERR err;

   //Make sure the operating system is running
   if(OSRunning == OS_STATE_OS_RUNNING)
   {
      //Properly dispose the specified mutex
      OSMutexDel(mutex, OS_OPT_DEL_ALWAYS, &err);
   }
}


/**
 * @brief Acquire ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osAcquireMutex(OsMutex *mutex)
{
   OS_ERR err;

   //Obtain ownership of the mutex object
   OSMutexPend(mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
}


/**
 * @brief Release ownership of the specified mutex object
 * @param[in] mutex Pointer to the mutex object
 **/

void osReleaseMutex(OsMutex *mutex)
{
   OS_ERR err;

   //Release ownership of the mutex object
   OSMutexPost(mutex, OS_OPT_POST_NONE, &err);
}


/**
 * @brief Retrieve system time
 * @return Number of milliseconds elapsed since the system was last started
 **/

systime_t osGetSystemTime(void)
{
   OS_ERR err;
   systime_t time;

   //Get current tick count
   time = OSTimeGet(&err);

   //Convert system ticks to milliseconds
   return OS_SYSTICKS_TO_MS(time);
}


/**
 * @brief Allocate a memory block
 * @param[in] size Bytes to allocate
 * @return A pointer to the allocated memory block or NULL if
 *   there is insufficient memory available
 **/

__weak_func void *osAllocMem(size_t size)
{
   void *p;

   //Enter critical section
   osSuspendAllTasks();
   //Allocate a memory block
   p = malloc(size);
   //Leave critical section
   osResumeAllTasks();

   //Debug message
   TRACE_DEBUG("Allocating %" PRIuSIZE " bytes at 0x%08" PRIXPTR "\r\n",
      size, (uintptr_t) p);

   //Return a pointer to the newly allocated memory block
   return p;
}


/**
 * @brief Release a previously allocated memory block
 * @param[in] p Previously allocated memory block to be freed
 **/

__weak_func void osFreeMem(void *p)
{
   //Make sure the pointer is valid
   if(p != NULL)
   {
      //Debug message
      TRACE_DEBUG("Freeing memory at 0x%08" PRIXPTR "\r\n", (uintptr_t) p);

      //Enter critical section
      osSuspendAllTasks();
      //Free memory block
      free(p);
      //Leave critical section
      osResumeAllTasks();
   }
}