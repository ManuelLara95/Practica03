/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MKL25Z128xxx4'
- !!package 'MKL25Z128VLK4'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.1.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN0_IDX		0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX		1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX		2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX		3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX		4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX		5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX		6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX		7u   /*!< Pin number for pin 7 in a port */

#define PIN18_IDX		18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX		19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX		20u   /*!< Pin number for pin 20 in a port */

#define SOPT5_UART0RXSRC_UART_RX      0x00u   /*!< UART0 receive data source select: UART0_RX pin */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART0 transmit data source select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '28', peripheral: UART0, signal: TX, pin_signal: TSI0_CH3/PTA2/UART0_TX/TPM2_CH1}
  - {pin_num: '27', peripheral: UART0, signal: RX, pin_signal: TSI0_CH2/PTA1/UART0_RX/TPM2_CH0}
  - {pin_num: '53', peripheral: GPIOB, signal: 'GPIO, 18', pin_signal: TSI0_CH11/PTB18/TPM2_CH0}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);						/* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);						/* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);						/* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);						/* Port B Clock Gate Control: Clock enabled */

  /* Init GPIOA 1:0 as Alt2*/
  PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt2);		/* PORTA1 (pin 27) is configured as UART0_RX */
  PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt2);		/* PORTA2 (pin 28) is configured as UART0_TX */

	/* Init GPIOC 7:0 as GPIO */
  PORT_SetPinMux(PORTC, PIN0_IDX, kPORT_MuxAsGpio);		/* PORTC0 is configured as PTC0 */
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAsGpio);		/* PORTC1 is configured as PTC1 */
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAsGpio);		/* PORTC2 is configured as PTC2 */
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAsGpio);		/* PORTC3 is configured as PTC3 */
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAsGpio);		/* PORTC4 is configured as PTC4 */
  PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAsGpio);		/* PORTC5 is configured as PTC5 */
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAsGpio);		/* PORTC6 is configured as PTC6 */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAsGpio);		/* PORTC7 is configured as PTC7 */

  /* Init GPIOB 2:0 as GPIO */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAsGpio);		/* PORTB0 is configured as PTB0 */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAsGpio);		/* PORTB1 is configured as PTB1 */
  PORT_SetPinMux(PORTB, PIN2_IDX, kPORT_MuxAsGpio);		/* PORTB2 is configured as PTB2 */

  /* Init GPIOE 3:0 as GPIO */
  PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAsGpio);					/* PORTE0 is configured as PTE0 */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAsGpio);					/* PORTE1 is configured as PTE1 */
  PORT_SetPinMux(PORTE, PIN2_IDX, kPORT_MuxAsGpio);					/* PORTE2 is configured as PTE2 */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_MuxAsGpio);					/* PORTE3 is configured as PTE2 */
  PORT_SetPinMux(PORTE, PIN20_IDX, kPORT_PinDisabledOrAnalog);		/* PORTE3 is configured as ADC0 */

  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK | SIM_SOPT5_UART0RXSRC_MASK)))	/* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       	/* UART0 transmit data source select: UART0_TX pin */
      | SIM_SOPT5_UART0RXSRC(SOPT5_UART0RXSRC_UART_RX)       	/* UART0 receive data source select: UART0_RX pin */
    );
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
