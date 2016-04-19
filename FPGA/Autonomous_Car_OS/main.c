/*************************************************************************
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.      *
* All rights reserved. All use of this software and documentation is     *
* subject to the License Agreement located at the end of this file below.*
**************************************************************************
* Description:                                                           *
* The following is a simple hello world program running MicroC/OS-II.The *
* purpose of the design is to be a very simple application that just     *
* demonstrates MicroC/OS-II running on NIOS II.The design doesn't account*
* for issues such as checking system call return codes. etc.             *
*                                                                        *
* Requirements:                                                          *
*   -Supported Example Hardware Platforms                                *
*     Standard                                                           *
*     Full Featured                                                      *
*     Low Cost                                                           *
*   -Supported Development Boards                                        *
*     Nios II Development Board, Stratix II Edition                      *
*     Nios Development Board, Stratix Professional Edition               *
*     Nios Development Board, Stratix Edition                            *
*     Nios Development Board, Cyclone Edition                            *
*   -System Library Settings                                             *
*     RTOS Type - MicroC/OS-II                                           *
*     Periodic System Timer                                              *
*   -Know Issues                                                         *
*     If this design is run on the ISS, terminal output will take several*
*     minutes per iteration.                                             *
**************************************************************************/


#include <stdio.h>
#include "includes.h"
#include <system.h>
#include <nios2.h>
#include "ultrasound.h"
#include "uart.h"
#include "motor.h"
#include "communication.h"
//#include "interrupt.h"
#include "gps.h"
#include <altera_avalon_pio_regs.h>
#include "altera_avalon_timer_regs.h"
#include <altera_avalon_uart_regs.h>
#include <io.h>
#include <sys/alt_irq.h>

/* Definition of Task Stacks */
#define   TASK_STACKSIZE       2048
OS_STK    task1_stk[TASK_STACKSIZE];
OS_STK    task2_stk[TASK_STACKSIZE];
OS_STK    task3_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */

#define TASK1_PRIORITY      1
#define TASK2_PRIORITY      2
#define TASK3_PRIORITY      5

/* Semaphore define */
OS_EVENT *isr_signal; //timer ISR signal to task 1
OS_EVENT *isr_signal2; //timer ISR signal to task 1

/* Prints "Hello World" and sleeps for three seconds */
void task1(void* pdata)
{
	NIOS2_WRITE_STATUS(0);
	int sr;
	unsigned char identifier;
	unsigned char data;
	INT8U return_code = OS_NO_ERR;

	unsigned int uiDistance1;

	unsigned char res_l, res_h;

	while (1)
	{
	    //wait for UART interrupt to permit execution
	    OSSemPend(isr_signal, 0, &return_code);

		sr = IORD_ALTERA_AVALON_UART_STATUS(NEW_ULTRASOUND3_BASE);

		if(sr & ALTERA_AVALON_UART_STATUS_RRDY_MSK)
		{
			data = uart_receive_byte(NEW_ULTRASOUND3_BASE);
			identifier = uart_receive_byte(NEW_ULTRASOUND3_BASE);

			switch (identifier)
			{
				case 1: setSpeedPerc(data-100); uart_send_string("Sp"); break;

				case 2: setSteeringPerc(data-100); uart_send_string("St"); break;

				default: uart_send_string("Er"); break;
			}

			IOWR_ALTERA_AVALON_UART_STATUS(NEW_ULTRASOUND3_BASE, 0);
		}

		uart_send_byte(NEW_ULTRASOUND0_BASE, 0xe8);
		//for (i = 0; i < 300; i++);
		uart_send_byte(NEW_ULTRASOUND0_BASE, 0x02);
		//for (i = 0; i < 300; i++)
			;
		if (0)
			uart_send_byte(NEW_ULTRASOUND0_BASE, 0xb8);
		else
			uart_send_byte(NEW_ULTRASOUND0_BASE, 0xb0);
		//for (i = 0; i < 300; i++);
		res_h = uart_receive_byte(NEW_ULTRASOUND0_BASE);
		// for (i = 0; i < 100; i++);
		res_l = uart_receive_byte(NEW_ULTRASOUND0_BASE);
		uiDistance1 = (((unsigned int) res_h) << 8) + res_l;

		printf("Dist: %d\n", uiDistance1);

		// Enable interrupt
		IOWR_ALTERA_AVALON_UART_CONTROL(NEW_ULTRASOUND3_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
	}
}

/* Prints "Hello World" and sleeps for three seconds */
void task2(void* pdata)
{

	while (1)
	{
		OSTimeDlyHMSM(0, 0, 6, 0);
	}
}

/* Prints "Hello World" and sleeps for three seconds */
void task3(void* pdata)
{
	  while (1)
	  {
		  OSTimeDlyHMSM(0, 0, 6, 0);
	  }
}

void nUART_ISR(void* nirq_isr_context)
{
	// Disable interrupt
	IOWR_ALTERA_AVALON_UART_CONTROL(NEW_ULTRASOUND3_BASE, 0x00);
	OSSemPost(isr_signal);
}

/* The main function creates two task and starts multi-tasking */
int main(void)
{
	ultrasound_init();
	communication_init();

	// Create a semaphore set at zero, so task3 will wait for nUART_ISR
	isr_signal = OSSemCreate(0);

	alt_ic_isr_register(
					NEW_ULTRASOUND3_IRQ_INTERRUPT_CONTROLLER_ID,
					NEW_ULTRASOUND3_IRQ,
			  nUART_ISR,
			  0x0,
			  0x0);

	printf("Hello!");
	uart_send_string("Hello!");

	OSTaskCreateExt(task1,
                  NULL,
                  (void *)&task1_stk[TASK_STACKSIZE-1],
                  TASK1_PRIORITY,
                  TASK1_PRIORITY,
                  task1_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);


	OSTaskCreateExt(task2,
                  NULL,
                  (void *)&task2_stk[TASK_STACKSIZE-1],
                  TASK2_PRIORITY,
                  TASK2_PRIORITY,
                  task2_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);


	OSTaskCreateExt(task3,
                  NULL,
                  (void *)&task3_stk[TASK_STACKSIZE-1],
                  TASK3_PRIORITY,
                  TASK3_PRIORITY,
                  task3_stk,
                  TASK_STACKSIZE,
                  NULL,
                  0);

	OSStart();
	return 0;
}

/******************************************************************************
*                                                                             *
* License Agreement                                                           *
*                                                                             *
* Copyright (c) 2004 Altera Corporation, San Jose, California, USA.           *
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
* Altera does not recommend, suggest or require that this reference design    *
* file be used in conjunction or combination with any other product.          *
******************************************************************************/
