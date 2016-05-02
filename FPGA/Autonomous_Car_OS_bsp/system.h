/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_qsys_0' in SOPC Builder design 'NIOS_Sys'
 * SOPC Builder design path: ../board_images/NIOS_Sys.sopcinfo
 *
 * Generated: Mon May 02 13:59:24 CEST 2016
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_qsys"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x02009820
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "tiny"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1a
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x00000020
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 0
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 0
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_ICACHE_SIZE 0
#define ALT_CPU_INST_ADDR_WIDTH 0x1a
#define ALT_CPU_NAME "nios2_qsys_0"
#define ALT_CPU_RESET_ADDR 0x02009000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x02009820
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "tiny"
#define NIOS2_DATA_ADDR_WIDTH 0x1a
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x00000020
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 0
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 0
#define NIOS2_ICACHE_LINE_SIZE_LOG2 0
#define NIOS2_ICACHE_SIZE 0
#define NIOS2_INST_ADDR_WIDTH 0x1a
#define NIOS2_RESET_ADDR 0x02009000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __2_CHANNEL_PWM
#define __ALTERA_AVALON_EPCS_FLASH_CONTROLLER
#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PERFORMANCE_COUNTER
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_TIMER
#define __ALTERA_AVALON_UART
#define __ALTERA_NIOS2_QSYS
#define __PULSE_SENOR_READ
#define __TERASIC_ADC_READ
#define __ULTRASOUND_INTERFACE


/*
 * New_UltraSound0 configuration
 *
 */

#define ALT_MODULE_CLASS_New_UltraSound0 altera_avalon_uart
#define NEW_ULTRASOUND0_BASE 0x200a220
#define NEW_ULTRASOUND0_BAUD 9600
#define NEW_ULTRASOUND0_DATA_BITS 8
#define NEW_ULTRASOUND0_FIXED_BAUD 1
#define NEW_ULTRASOUND0_FREQ 50000000
#define NEW_ULTRASOUND0_IRQ 6
#define NEW_ULTRASOUND0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define NEW_ULTRASOUND0_NAME "/dev/New_UltraSound0"
#define NEW_ULTRASOUND0_PARITY 'N'
#define NEW_ULTRASOUND0_SIM_CHAR_STREAM ""
#define NEW_ULTRASOUND0_SIM_TRUE_BAUD 0
#define NEW_ULTRASOUND0_SPAN 32
#define NEW_ULTRASOUND0_STOP_BITS 1
#define NEW_ULTRASOUND0_SYNC_REG_DEPTH 2
#define NEW_ULTRASOUND0_TYPE "altera_avalon_uart"
#define NEW_ULTRASOUND0_USE_CTS_RTS 0
#define NEW_ULTRASOUND0_USE_EOP_REGISTER 0


/*
 * New_UltraSound1 configuration
 *
 */

#define ALT_MODULE_CLASS_New_UltraSound1 altera_avalon_uart
#define NEW_ULTRASOUND1_BASE 0x200a200
#define NEW_ULTRASOUND1_BAUD 9600
#define NEW_ULTRASOUND1_DATA_BITS 8
#define NEW_ULTRASOUND1_FIXED_BAUD 1
#define NEW_ULTRASOUND1_FREQ 50000000
#define NEW_ULTRASOUND1_IRQ 7
#define NEW_ULTRASOUND1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define NEW_ULTRASOUND1_NAME "/dev/New_UltraSound1"
#define NEW_ULTRASOUND1_PARITY 'N'
#define NEW_ULTRASOUND1_SIM_CHAR_STREAM ""
#define NEW_ULTRASOUND1_SIM_TRUE_BAUD 0
#define NEW_ULTRASOUND1_SPAN 32
#define NEW_ULTRASOUND1_STOP_BITS 1
#define NEW_ULTRASOUND1_SYNC_REG_DEPTH 2
#define NEW_ULTRASOUND1_TYPE "altera_avalon_uart"
#define NEW_ULTRASOUND1_USE_CTS_RTS 0
#define NEW_ULTRASOUND1_USE_EOP_REGISTER 0


/*
 * New_UltraSound2 configuration
 *
 */

#define ALT_MODULE_CLASS_New_UltraSound2 altera_avalon_uart
#define NEW_ULTRASOUND2_BASE 0x200a1e0
#define NEW_ULTRASOUND2_BAUD 9600
#define NEW_ULTRASOUND2_DATA_BITS 8
#define NEW_ULTRASOUND2_FIXED_BAUD 1
#define NEW_ULTRASOUND2_FREQ 50000000
#define NEW_ULTRASOUND2_IRQ 8
#define NEW_ULTRASOUND2_IRQ_INTERRUPT_CONTROLLER_ID 0
#define NEW_ULTRASOUND2_NAME "/dev/New_UltraSound2"
#define NEW_ULTRASOUND2_PARITY 'N'
#define NEW_ULTRASOUND2_SIM_CHAR_STREAM ""
#define NEW_ULTRASOUND2_SIM_TRUE_BAUD 0
#define NEW_ULTRASOUND2_SPAN 32
#define NEW_ULTRASOUND2_STOP_BITS 1
#define NEW_ULTRASOUND2_SYNC_REG_DEPTH 2
#define NEW_ULTRASOUND2_TYPE "altera_avalon_uart"
#define NEW_ULTRASOUND2_USE_CTS_RTS 0
#define NEW_ULTRASOUND2_USE_EOP_REGISTER 0


/*
 * New_UltraSound3 configuration
 *
 */

#define ALT_MODULE_CLASS_New_UltraSound3 altera_avalon_uart
#define NEW_ULTRASOUND3_BASE 0x200a1c0
#define NEW_ULTRASOUND3_BAUD 9600
#define NEW_ULTRASOUND3_DATA_BITS 8
#define NEW_ULTRASOUND3_FIXED_BAUD 1
#define NEW_ULTRASOUND3_FREQ 50000000
#define NEW_ULTRASOUND3_IRQ 9
#define NEW_ULTRASOUND3_IRQ_INTERRUPT_CONTROLLER_ID 0
#define NEW_ULTRASOUND3_NAME "/dev/New_UltraSound3"
#define NEW_ULTRASOUND3_PARITY 'N'
#define NEW_ULTRASOUND3_SIM_CHAR_STREAM ""
#define NEW_ULTRASOUND3_SIM_TRUE_BAUD 0
#define NEW_ULTRASOUND3_SPAN 32
#define NEW_ULTRASOUND3_STOP_BITS 1
#define NEW_ULTRASOUND3_SYNC_REG_DEPTH 2
#define NEW_ULTRASOUND3_TYPE "altera_avalon_uart"
#define NEW_ULTRASOUND3_USE_CTS_RTS 0
#define NEW_ULTRASOUND3_USE_EOP_REGISTER 0


/*
 * PWM0 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM0 2_channel_PWM
#define PWM0_BASE 0x200a2c0
#define PWM0_IRQ -1
#define PWM0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM0_NAME "/dev/PWM0"
#define PWM0_SPAN 32
#define PWM0_TYPE "2_channel_PWM"


/*
 * PWM1 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM1 2_channel_PWM
#define PWM1_BASE 0x200a2a0
#define PWM1_IRQ -1
#define PWM1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM1_NAME "/dev/PWM1"
#define PWM1_SPAN 32
#define PWM1_TYPE "2_channel_PWM"


/*
 * PWM2 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM2 2_channel_PWM
#define PWM2_BASE 0x200a260
#define PWM2_IRQ -1
#define PWM2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM2_NAME "/dev/PWM2"
#define PWM2_SPAN 32
#define PWM2_TYPE "2_channel_PWM"


/*
 * PWM3 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM3 2_channel_PWM
#define PWM3_BASE 0x200a240
#define PWM3_IRQ -1
#define PWM3_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM3_NAME "/dev/PWM3"
#define PWM3_SPAN 32
#define PWM3_TYPE "2_channel_PWM"


/*
 * PWM4 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM4 2_channel_PWM
#define PWM4_BASE 0x200a1a0
#define PWM4_IRQ -1
#define PWM4_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM4_NAME "/dev/PWM4"
#define PWM4_SPAN 32
#define PWM4_TYPE "2_channel_PWM"


/*
 * PWM5 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM5 2_channel_PWM
#define PWM5_BASE 0x200a180
#define PWM5_IRQ -1
#define PWM5_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM5_NAME "/dev/PWM5"
#define PWM5_SPAN 32
#define PWM5_TYPE "2_channel_PWM"


/*
 * PWM6 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM6 2_channel_PWM
#define PWM6_BASE 0x200a160
#define PWM6_IRQ -1
#define PWM6_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM6_NAME "/dev/PWM6"
#define PWM6_SPAN 32
#define PWM6_TYPE "2_channel_PWM"


/*
 * PWM7 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM7 2_channel_PWM
#define PWM7_BASE 0x200a140
#define PWM7_IRQ -1
#define PWM7_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM7_NAME "/dev/PWM7"
#define PWM7_SPAN 32
#define PWM7_TYPE "2_channel_PWM"


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone IV E"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart_0"
#define ALT_STDERR_BASE 0x200a3d0
#define ALT_STDERR_DEV jtag_uart_0
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_0"
#define ALT_STDIN_BASE 0x200a3d0
#define ALT_STDIN_DEV jtag_uart_0
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_0"
#define ALT_STDOUT_BASE 0x200a3d0
#define ALT_STDOUT_DEV jtag_uart_0
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "NIOS_Sys"


/*
 * Ultrasound0 configuration
 *
 */

#define ALT_MODULE_CLASS_Ultrasound0 Ultrasound_interface
#define ULTRASOUND0_BASE 0x200a390
#define ULTRASOUND0_IRQ -1
#define ULTRASOUND0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ULTRASOUND0_NAME "/dev/Ultrasound0"
#define ULTRASOUND0_SPAN 16
#define ULTRASOUND0_TYPE "Ultrasound_interface"


/*
 * Ultrasound1 configuration
 *
 */

#define ALT_MODULE_CLASS_Ultrasound1 Ultrasound_interface
#define ULTRASOUND1_BASE 0x200a380
#define ULTRASOUND1_IRQ -1
#define ULTRASOUND1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ULTRASOUND1_NAME "/dev/Ultrasound1"
#define ULTRASOUND1_SPAN 16
#define ULTRASOUND1_TYPE "Ultrasound_interface"


/*
 * Ultrasound2 configuration
 *
 */

#define ALT_MODULE_CLASS_Ultrasound2 Ultrasound_interface
#define ULTRASOUND2_BASE 0x200a340
#define ULTRASOUND2_IRQ -1
#define ULTRASOUND2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ULTRASOUND2_NAME "/dev/Ultrasound2"
#define ULTRASOUND2_SPAN 16
#define ULTRASOUND2_TYPE "Ultrasound_interface"


/*
 * Ultrasound3 configuration
 *
 */

#define ALT_MODULE_CLASS_Ultrasound3 Ultrasound_interface
#define ULTRASOUND3_BASE 0x200a330
#define ULTRASOUND3_IRQ -1
#define ULTRASOUND3_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ULTRASOUND3_NAME "/dev/Ultrasound3"
#define ULTRASOUND3_SPAN 16
#define ULTRASOUND3_TYPE "Ultrasound_interface"


/*
 * Ultrasound4 configuration
 *
 */

#define ALT_MODULE_CLASS_Ultrasound4 Ultrasound_interface
#define ULTRASOUND4_BASE 0x200a320
#define ULTRASOUND4_IRQ -1
#define ULTRASOUND4_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ULTRASOUND4_NAME "/dev/Ultrasound4"
#define ULTRASOUND4_SPAN 16
#define ULTRASOUND4_TYPE "Ultrasound_interface"


/*
 * Ultrasound5 configuration
 *
 */

#define ALT_MODULE_CLASS_Ultrasound5 Ultrasound_interface
#define ULTRASOUND5_BASE 0x200a310
#define ULTRASOUND5_IRQ -1
#define ULTRASOUND5_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ULTRASOUND5_NAME "/dev/Ultrasound5"
#define ULTRASOUND5_SPAN 16
#define ULTRASOUND5_TYPE "Ultrasound_interface"


/*
 * adc_spi_read configuration
 *
 */

#define ADC_SPI_READ_BASE 0x200a3d8
#define ADC_SPI_READ_IRQ -1
#define ADC_SPI_READ_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ADC_SPI_READ_NAME "/dev/adc_spi_read"
#define ADC_SPI_READ_SPAN 4
#define ADC_SPI_READ_TYPE "TERASIC_ADC_READ"
#define ALT_MODULE_CLASS_adc_spi_read TERASIC_ADC_READ


/*
 * encoder0 configuration
 *
 */

#define ALT_MODULE_CLASS_encoder0 pulse_senor_read
#define ENCODER0_BASE 0x200a370
#define ENCODER0_IRQ -1
#define ENCODER0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ENCODER0_NAME "/dev/encoder0"
#define ENCODER0_SPAN 16
#define ENCODER0_TYPE "pulse_senor_read"


/*
 * encoder1 configuration
 *
 */

#define ALT_MODULE_CLASS_encoder1 pulse_senor_read
#define ENCODER1_BASE 0x200a360
#define ENCODER1_IRQ -1
#define ENCODER1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ENCODER1_NAME "/dev/encoder1"
#define ENCODER1_SPAN 16
#define ENCODER1_TYPE "pulse_senor_read"


/*
 * epcs_flash_controller_0 configuration
 *
 */

#define ALT_MODULE_CLASS_epcs_flash_controller_0 altera_avalon_epcs_flash_controller
#define EPCS_FLASH_CONTROLLER_0_BASE 0x2009000
#define EPCS_FLASH_CONTROLLER_0_IRQ 5
#define EPCS_FLASH_CONTROLLER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define EPCS_FLASH_CONTROLLER_0_NAME "/dev/epcs_flash_controller_0"
#define EPCS_FLASH_CONTROLLER_0_REGISTER_OFFSET 1024
#define EPCS_FLASH_CONTROLLER_0_SPAN 2048
#define EPCS_FLASH_CONTROLLER_0_TYPE "altera_avalon_epcs_flash_controller"


/*
 * hal configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_0 altera_avalon_jtag_uart
#define JTAG_UART_0_BASE 0x200a3d0
#define JTAG_UART_0_IRQ 3
#define JTAG_UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_0_NAME "/dev/jtag_uart_0"
#define JTAG_UART_0_READ_DEPTH 64
#define JTAG_UART_0_READ_THRESHOLD 8
#define JTAG_UART_0_SPAN 8
#define JTAG_UART_0_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_0_WRITE_DEPTH 64
#define JTAG_UART_0_WRITE_THRESHOLD 8


/*
 * key configuration
 *
 */

#define ALT_MODULE_CLASS_key altera_avalon_pio
#define KEY_BASE 0x200a3b0
#define KEY_BIT_CLEARING_EDGE_REGISTER 0
#define KEY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEY_CAPTURE 1
#define KEY_DATA_WIDTH 2
#define KEY_DO_TEST_BENCH_WIRING 0
#define KEY_DRIVEN_SIM_VALUE 0
#define KEY_EDGE_TYPE "FALLING"
#define KEY_FREQ 50000000
#define KEY_HAS_IN 1
#define KEY_HAS_OUT 0
#define KEY_HAS_TRI 0
#define KEY_IRQ 1
#define KEY_IRQ_INTERRUPT_CONTROLLER_ID 0
#define KEY_IRQ_TYPE "EDGE"
#define KEY_NAME "/dev/key"
#define KEY_RESET_VALUE 0
#define KEY_SPAN 16
#define KEY_TYPE "altera_avalon_pio"


/*
 * led configuration
 *
 */

#define ALT_MODULE_CLASS_led altera_avalon_pio
#define LED_BASE 0x200a3c0
#define LED_BIT_CLEARING_EDGE_REGISTER 0
#define LED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_CAPTURE 0
#define LED_DATA_WIDTH 8
#define LED_DO_TEST_BENCH_WIRING 0
#define LED_DRIVEN_SIM_VALUE 0
#define LED_EDGE_TYPE "NONE"
#define LED_FREQ 50000000
#define LED_HAS_IN 0
#define LED_HAS_OUT 1
#define LED_HAS_TRI 0
#define LED_IRQ -1
#define LED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LED_IRQ_TYPE "NONE"
#define LED_NAME "/dev/led"
#define LED_RESET_VALUE 0
#define LED_SPAN 16
#define LED_TYPE "altera_avalon_pio"


/*
 * onchip_memory2_0 configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory2_0 altera_avalon_onchip_memory2
#define ONCHIP_MEMORY2_0_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY2_0_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY2_0_BASE 0x2004000
#define ONCHIP_MEMORY2_0_CONTENTS_INFO ""
#define ONCHIP_MEMORY2_0_DUAL_PORT 0
#define ONCHIP_MEMORY2_0_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_INIT_CONTENTS_FILE "NIOS_Sys_onchip_memory2_0"
#define ONCHIP_MEMORY2_0_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY2_0_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY2_0_IRQ -1
#define ONCHIP_MEMORY2_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY2_0_NAME "/dev/onchip_memory2_0"
#define ONCHIP_MEMORY2_0_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY2_0_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY2_0_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY2_0_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY2_0_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY2_0_SIZE_VALUE 16384
#define ONCHIP_MEMORY2_0_SPAN 16384
#define ONCHIP_MEMORY2_0_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY2_0_WRITABLE 1


/*
 * performance_counter configuration
 *
 */

#define ALT_MODULE_CLASS_performance_counter altera_avalon_performance_counter
#define PERFORMANCE_COUNTER_BASE 0x200a000
#define PERFORMANCE_COUNTER_HOW_MANY_SECTIONS 3
#define PERFORMANCE_COUNTER_IRQ -1
#define PERFORMANCE_COUNTER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PERFORMANCE_COUNTER_NAME "/dev/performance_counter"
#define PERFORMANCE_COUNTER_SPAN 64
#define PERFORMANCE_COUNTER_TYPE "altera_avalon_performance_counter"


/*
 * pio_0 configuration
 *
 */

#define ALT_MODULE_CLASS_pio_0 altera_avalon_pio
#define PIO_0_BASE 0x200a350
#define PIO_0_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_0_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_0_CAPTURE 0
#define PIO_0_DATA_WIDTH 8
#define PIO_0_DO_TEST_BENCH_WIRING 0
#define PIO_0_DRIVEN_SIM_VALUE 0
#define PIO_0_EDGE_TYPE "NONE"
#define PIO_0_FREQ 50000000
#define PIO_0_HAS_IN 0
#define PIO_0_HAS_OUT 1
#define PIO_0_HAS_TRI 0
#define PIO_0_IRQ -1
#define PIO_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_0_IRQ_TYPE "NONE"
#define PIO_0_NAME "/dev/pio_0"
#define PIO_0_RESET_VALUE 0
#define PIO_0_SPAN 16
#define PIO_0_TYPE "altera_avalon_pio"


/*
 * pio_1 configuration
 *
 */

#define ALT_MODULE_CLASS_pio_1 altera_avalon_pio
#define PIO_1_BASE 0x200a300
#define PIO_1_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_1_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_1_CAPTURE 0
#define PIO_1_DATA_WIDTH 4
#define PIO_1_DO_TEST_BENCH_WIRING 0
#define PIO_1_DRIVEN_SIM_VALUE 0
#define PIO_1_EDGE_TYPE "NONE"
#define PIO_1_FREQ 50000000
#define PIO_1_HAS_IN 1
#define PIO_1_HAS_OUT 0
#define PIO_1_HAS_TRI 0
#define PIO_1_IRQ -1
#define PIO_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_1_IRQ_TYPE "NONE"
#define PIO_1_NAME "/dev/pio_1"
#define PIO_1_RESET_VALUE 0
#define PIO_1_SPAN 16
#define PIO_1_TYPE "altera_avalon_pio"


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x0
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x18
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 9
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 13
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 1
#define SDRAM_SPAN 33554432
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0


/*
 * sw configuration
 *
 */

#define ALT_MODULE_CLASS_sw altera_avalon_pio
#define SW_BASE 0x200a3a0
#define SW_BIT_CLEARING_EDGE_REGISTER 0
#define SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SW_CAPTURE 1
#define SW_DATA_WIDTH 4
#define SW_DO_TEST_BENCH_WIRING 0
#define SW_DRIVEN_SIM_VALUE 0
#define SW_EDGE_TYPE "ANY"
#define SW_FREQ 50000000
#define SW_HAS_IN 1
#define SW_HAS_OUT 0
#define SW_HAS_TRI 0
#define SW_IRQ 2
#define SW_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SW_IRQ_TYPE "EDGE"
#define SW_NAME "/dev/sw"
#define SW_RESET_VALUE 0
#define SW_SPAN 16
#define SW_TYPE "altera_avalon_pio"


/*
 * timer configuration
 *
 */

#define ALT_MODULE_CLASS_timer altera_avalon_timer
#define TIMER_ALWAYS_RUN 0
#define TIMER_BASE 0x200a2e0
#define TIMER_COUNTER_SIZE 32
#define TIMER_FIXED_PERIOD 0
#define TIMER_FREQ 50000000
#define TIMER_IRQ 0
#define TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_LOAD_VALUE 49999
#define TIMER_MULT 0.001
#define TIMER_NAME "/dev/timer"
#define TIMER_PERIOD 1
#define TIMER_PERIOD_UNITS "ms"
#define TIMER_RESET_OUTPUT 0
#define TIMER_SNAPSHOT 1
#define TIMER_SPAN 32
#define TIMER_TICKS_PER_SEC 1000.0
#define TIMER_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TYPE "altera_avalon_timer"


/*
 * uart_0 configuration
 *
 */

#define ALT_MODULE_CLASS_uart_0 altera_avalon_uart
#define UART_0_BASE 0x200a280
#define UART_0_BAUD 115200
#define UART_0_DATA_BITS 8
#define UART_0_FIXED_BAUD 1
#define UART_0_FREQ 50000000
#define UART_0_IRQ 4
#define UART_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define UART_0_NAME "/dev/uart_0"
#define UART_0_PARITY 'N'
#define UART_0_SIM_CHAR_STREAM ""
#define UART_0_SIM_TRUE_BAUD 0
#define UART_0_SPAN 32
#define UART_0_STOP_BITS 1
#define UART_0_SYNC_REG_DEPTH 2
#define UART_0_TYPE "altera_avalon_uart"
#define UART_0_USE_CTS_RTS 0
#define UART_0_USE_EOP_REGISTER 0


/*
 * ucosii configuration
 *
 */

#define OS_ARG_CHK_EN 1
#define OS_CPU_HOOKS_EN 1
#define OS_DEBUG_EN 1
#define OS_EVENT_NAME_SIZE 32
#define OS_FLAGS_NBITS 16
#define OS_FLAG_ACCEPT_EN 1
#define OS_FLAG_DEL_EN 1
#define OS_FLAG_EN 1
#define OS_FLAG_NAME_SIZE 32
#define OS_FLAG_QUERY_EN 1
#define OS_FLAG_WAIT_CLR_EN 1
#define OS_LOWEST_PRIO 20
#define OS_MAX_EVENTS 60
#define OS_MAX_FLAGS 20
#define OS_MAX_MEM_PART 60
#define OS_MAX_QS 20
#define OS_MAX_TASKS 10
#define OS_MBOX_ACCEPT_EN 1
#define OS_MBOX_DEL_EN 1
#define OS_MBOX_EN 1
#define OS_MBOX_POST_EN 1
#define OS_MBOX_POST_OPT_EN 1
#define OS_MBOX_QUERY_EN 1
#define OS_MEM_EN 1
#define OS_MEM_NAME_SIZE 32
#define OS_MEM_QUERY_EN 1
#define OS_MUTEX_ACCEPT_EN 1
#define OS_MUTEX_DEL_EN 1
#define OS_MUTEX_EN 1
#define OS_MUTEX_QUERY_EN 1
#define OS_Q_ACCEPT_EN 1
#define OS_Q_DEL_EN 1
#define OS_Q_EN 1
#define OS_Q_FLUSH_EN 1
#define OS_Q_POST_EN 1
#define OS_Q_POST_FRONT_EN 1
#define OS_Q_POST_OPT_EN 1
#define OS_Q_QUERY_EN 1
#define OS_SCHED_LOCK_EN 1
#define OS_SEM_ACCEPT_EN 1
#define OS_SEM_DEL_EN 1
#define OS_SEM_EN 1
#define OS_SEM_QUERY_EN 1
#define OS_SEM_SET_EN 1
#define OS_TASK_CHANGE_PRIO_EN 1
#define OS_TASK_CREATE_EN 1
#define OS_TASK_CREATE_EXT_EN 1
#define OS_TASK_DEL_EN 1
#define OS_TASK_IDLE_STK_SIZE 512
#define OS_TASK_NAME_SIZE 32
#define OS_TASK_PROFILE_EN 1
#define OS_TASK_QUERY_EN 1
#define OS_TASK_STAT_EN 1
#define OS_TASK_STAT_STK_CHK_EN 1
#define OS_TASK_STAT_STK_SIZE 512
#define OS_TASK_SUSPEND_EN 1
#define OS_TASK_SW_HOOK_EN 1
#define OS_TASK_TMR_PRIO 0
#define OS_TASK_TMR_STK_SIZE 512
#define OS_THREAD_SAFE_NEWLIB 1
#define OS_TICKS_PER_SEC TIMER_TICKS_PER_SEC
#define OS_TICK_STEP_EN 1
#define OS_TIME_DLY_HMSM_EN 1
#define OS_TIME_DLY_RESUME_EN 1
#define OS_TIME_GET_SET_EN 1
#define OS_TIME_TICK_HOOK_EN 1
#define OS_TMR_CFG_MAX 16
#define OS_TMR_CFG_NAME_SIZE 16
#define OS_TMR_CFG_TICKS_PER_SEC 10
#define OS_TMR_CFG_WHEEL_SIZE 2
#define OS_TMR_EN 0

#endif /* __SYSTEM_H_ */
