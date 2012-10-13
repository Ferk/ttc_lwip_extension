/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __SYS_RTXC_H__
#define __SYS_RTXC_H__

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define SYS_MBOX_NULL (xQueueHandle)0
#define SYS_SEM_NULL  (xSemaphoreHandle)0
#define SYS_DEFAULT_THREAD_STACK_DEPTH	configMINIMAL_STACK_SIZE
#define SYS_THREAD_NULL         NULL
#define SIO_FD_NULL             ( sio_fd_t )NULL

#define sys_mbox_valid( x ) ( ( ( *x ) == NULL) ? pdFALSE : pdTRUE )
#define sys_mbox_set_invalid( x ) ( ( *x ) = NULL )
#define sys_sem_valid( x ) ( ( ( *x ) == NULL) ? pdFALSE : pdTRUE )
#define sys_sem_set_invalid( x ) ( ( *x ) = NULL )



typedef xSemaphoreHandle sys_sem_t;
typedef xQueueHandle sys_mbox_t;
typedef xTaskHandle sys_thread_t;

///
typedef xSemaphoreHandle sys_mutex_t;
///

typedef struct _sys_arch_state_t
{
	// Task creation data.
	char cTaskName[configMAX_TASK_NAME_LEN];
	unsigned short nStackDepth;
	unsigned short nTaskCount;
} sys_arch_state_t;

//extern sys_arch_state_t s_sys_arch_state;

void sys_set_default_state();
void sys_set_state(signed char *pTaskName, unsigned short nStackSize);

sys_thread_t    sys_arch_thread_new( void ( *thread ) ( void *arg ), void *arg,
                                     int prio, size_t ssize );
sys_thread_t    sys_arch_thread_current( void );
void            sys_arch_thread_remove( sys_thread_t hdl );
void            sys_assert( const char *const msg );
void            sys_debug( const char *const fmt, ... );




// /*
//   Is called to initialize the sys_arch layer.
// */
// void sys_init(void);

// /*
//   Creates a new semaphore. The semaphore is allocated to the memory that 'sem'
//   points to (which can be both a pointer or the actual OS structure).
//   The "count" argument specifies the initial state of the semaphore (which is
//   either 0 or 1).
//   If the semaphore has been created, ERR_OK should be returned. Returning any
//   other error will provide a hint what went wrong, but except for assertions,
//   no real error handling is implemented.
// */
// err_t sys_sem_new(sys_sem_t *sem, u8_t count);

// /*
//   Deallocates a semaphore.
// */
// void sys_sem_free(sys_sem_t *sem);

// /*
//   Signals a semaphore.

// */
// void sys_sem_signal(sys_sem_t *sem);

// /*
//   Blocks the thread while waiting for the semaphore to be
//   signaled. If the "timeout" argument is non-zero, the thread should
//   only be blocked for the specified time (measured in
//   milliseconds). If the "timeout" argument is zero, the thread should be
//   blocked until the semaphore is signalled.

//   If the timeout argument is non-zero, the return value is the number of
//   milliseconds spent waiting for the semaphore to be signaled. If the
//   semaphore wasn't signaled within the specified time, the return value is
//   SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
//   (i.e., it was already signaled), the function may return zero.

//   Notice that lwIP implements a function with a similar name,
//   sys_sem_wait(), that uses the sys_arch_sem_wait() function.
// */
// u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)

// /*
//   Returns 1 if the semaphore is valid, 0 if it is not valid.
//   When using pointers, a simple way is to check the pointer for != NULL.
//   When directly using OS structures, implementing this may be more complex.
//   This may also be a define, in which case the function is not prototyped.
// */
// int sys_sem_valid(sys_sem_t *sem);

// /*
//   Invalidate a semaphore so that sys_sem_valid() returns 0.
//   ATTENTION: This does NOT mean that the semaphore shall be deallocated:
//   sys_sem_free() is always called before calling this function!
//   This may also be a define, in which case the function is not prototyped.
// */
// void sys_sem_set_invalid(sys_sem_t *sem);

// /*
//   Creates an empty mailbox for maximum "size" elements. Elements stored
//   in mailboxes are pointers. You have to define macros "_MBOX_SIZE"
//   in your lwipopts.h, or ignore this parameter in your implementation
//   and use a default size.
//   If the mailbox has been created, ERR_OK should be returned. Returning any
//   other error will provide a hint what went wrong, but except for assertions,
//   no real error handling is implemented.
// */
// err_t sys_mbox_new(sys_mbox_t *mbox, int size);

// /*
//   Deallocates a mailbox. If there are messages still present in the
//   mailbox when the mailbox is deallocated, it is an indication of a
//   programming error in lwIP and the developer should be notified.
// */
// void sys_mbox_free(sys_mbox_t *mbox);

// /*
//   Posts the "msg" to the mailbox. This function have to block until
//   the "msg" is really posted.
// */
// void sys_mbox_post(sys_mbox_t *mbox, void *msg);

// /*
// - err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg);

//   Try to post the "msg" to the mailbox. Returns ERR_MEM if this one
//   is full, else, ERR_OK if the "msg" is posted.

//   Blocks the thread until a message arrives in the mailbox, but does
//   not block the thread longer than "timeout" milliseconds (similar to
//   the sys_arch_sem_wait() function). If "timeout" is 0, the thread should
//   be blocked until a message arrives. The "msg" argument is a result
//   parameter that is set by the function (i.e., by doing "*msg =
//   ptr"). The "msg" parameter maybe NULL to indicate that the message
//   should be dropped.

//   The return values are the same as for the sys_arch_sem_wait() function:
//   Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
//   timeout.

//   Note that a function with a similar name, sys_mbox_fetch(), is
//   implemented by lwIP. 
// */
// u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout);

// /*
//   This is similar to sys_arch_mbox_fetch, however if a message is not
//   present in the mailbox, it immediately returns with the code
//   SYS_MBOX_EMPTY. On success 0 is returned.

//   To allow for efficient implementations, this can be defined as a
//   function-like macro in sys_arch.h instead of a normal function. For
//   example, a naive implementation could be:
//     #define sys_arch_mbox_tryfetch(mbox,msg) sys_arch_mbox_fetch(mbox,msg,1);
//   although this would introduce unnecessary delays.
// */
// u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg);

// /*
//   Returns 1 if the mailbox is valid, 0 if it is not valid.
//   When using pointers, a simple way is to check the pointer for != NULL.
//   When directly using OS structures, implementing this may be more complex.
//   This may also be a define, in which case the function is not prototyped.
// */
//int sys_mbox_valid(sys_mbox_t *mbox);

// /*
//   Invalidate a mailbox so that sys_mbox_valid() returns 0.
//   ATTENTION: This does NOT mean that the mailbox shall be deallocated:
//   sys_mbox_free() is always called before calling this function!
//   This may also be a define, in which case the function is not prototyped.

// If threads are supported by the underlying operating system and if
// such functionality is needed in lwIP, the following function will have
// to be implemented as well:
// */
// void sys_mbox_set_invalid(sys_mbox_t *mbox);

// /*
// - sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio);

//   Starts a new thread named "name" with priority "prio" that will begin its
//   execution in the function "thread()". The "arg" argument will be passed as an
//   argument to the thread() function. The stack size to used for this thread is
//   the "stacksize" parameter. The id of the new thread is returned. Both the id
//   and the priority are system dependent.

//   This optional function does a "fast" critical region protection and returns
//   the previous protection level. This function is only called during very short
//   critical regions. An embedded system which supports ISR-based drivers might
//   want to implement this function by disabling interrupts. Task-based systems
//   might want to implement this by using a mutex or disabling tasking. This
//   function should support recursive calls from the same task or interrupt. In
//   other words, sys_arch_protect() could be called while already protected. In
//   that case the return value indicates that it is already protected.

//   sys_arch_protect() is only required if your port is supporting an operating
//   system.
// */
// //sys_prot_t sys_arch_protect(void);


#endif /* __SYS_RTXC_H__ */

