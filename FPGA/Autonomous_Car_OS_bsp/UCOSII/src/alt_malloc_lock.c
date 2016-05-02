/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2003 Altera Corporation, San Jose, California, USA.           *
* All rights reserved.                                                        *
*                                                                             *
* Permission is hereby granted, free of charge, to any person obtaining a     *
* copy of this software and associated documentation files (the "Software"),  *
* to deal in the Software without restriction, including without limitation   *
* the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
* and/or sell copies of the Software, and to permit persons to whom the       *
* Software is furnished to do so, subject to the following conditions:        *
*                                                                             *
* The above copyright notice and this permission notice shall be included in  *
* all copies or substantial portions of the Software.                         *
*                                                                             *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING     *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER         *
* DEALINGS IN THE SOFTWARE.                                                   *
*                                                                             *
* This agreement shall be governed in all respects by the laws of the State   *
* of California and by the laws of the United States of America.              *
*                                                                             *
******************************************************************************/

#include "system.h"

/*
 * These are the malloc lock/unlock stubs required by newlib. These are 
 * used to make accesses to the heap thread safe. Note that 
 * this implementation requires that the heap is never manipulated 
 * by an interrupt service routine.
 */

#include <reent.h>

#include "includes.h"

/* semaphore to protect the heap */

OS_EVENT *alt_heapsem;


#if OS_THREAD_SAFE_NEWLIB
/* id of the task that is currently manipulating the heap */

static int lockid = -1;

/* number of times __malloc_lock has recursed */

static int locks;
#endif /* OS_THREAD_SAFE_NEWLIB */

/*
 *
 */

void __malloc_lock ( struct _reent *_r )
{
#if OS_THREAD_SAFE_NEWLIB
  OS_TCB tcb;
  OS_SEM_DATA semdata;
  INT8U err;
  int id;
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif  
  
  if (OSRunning != OS_TRUE)
      return;

  /* use our priority as a task id */

  err = OSTaskQuery( OS_PRIO_SELF, &tcb );
  if (err != OS_NO_ERR)
    return;

  id = tcb.OSTCBPrio;
   
  /* see if we own the heap already */

  OSSemQuery( alt_heapsem, &semdata );
  
  OS_ENTER_CRITICAL();

  if( !semdata.OSCnt && id == lockid ) 
  {
    /* we do; just count the recursion */
    locks++;
    OS_EXIT_CRITICAL();
  }
  else 
  {
    /* wait on the other task to yield the heap, then claim ownership of it */
    OS_EXIT_CRITICAL();

    OSSemPend( alt_heapsem, 0, &err );
    locks  = 1;
    lockid = id;
  }

#endif /* OS_THREAD_SAFE_NEWLIB */
  return;
}

/*
 *
 */

void __malloc_unlock ( struct _reent *_r )
{
#if OS_THREAD_SAFE_NEWLIB

#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr = 0;
#endif 

  if (OSRunning != OS_TRUE)
      return;

  OS_ENTER_CRITICAL();
  if (locks == 0)
  {
      OS_EXIT_CRITICAL();
      return;
  }

  /* release the heap once the number of locks == the number of unlocks */
  if( (--locks) == 0 ) 
  {
    lockid = -1;
    OS_EXIT_CRITICAL();
    OSSemPost( alt_heapsem );
  }
  else
  {
      OS_EXIT_CRITICAL();
  }
  
#endif /* OS_THREAD_SAFE_NEWLIB */
}

