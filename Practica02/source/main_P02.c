/*
 **************************************************
 * 			UNIVERSIDAD DE GUADALAJARA
 * 				PIENSA Y TRABAJA
 **************************************************
 * @file    		main_P01.c
 * @brief   		Application entry point
 * SW Engineer	: 	Juan Manuel Lara Esparza
 **************************************************
 */
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "fsl_tpm.h"
#include "fsl_adc16.h"

/* TODO: MACROS & VARIABLES DEFINITION SECTION */
/* This is where general variables & macros section starts */
	//	MACROS:
	#define GPIOC_OUTPUTBITS	0xFF
	#define GPIOE_OUTPUTBITS	0x0F
	#define	GPIOB_INPUTBITS		0x07
	#define THREE_BUTTON_BITS	0x07
	#define DECIMAL_POINT		1u

	#define TWOpower12		0xFFF
	#define TWOpower16		0xFFFF

	#define CONST_VECES		5u
	#define	CYCLES_CHANGE	70u

	#define BIT0		0x01
	#define BIT1		0x02
	#define BIT2		0x04
	#define BIT3		0x08
	#define BIT4		0x10
	#define BIT5		0x20
	#define BIT20		0x80000

	#define CONTADOR_MANUAL		1u
	#define	CONTADOR_AUTO		2u
	#define MODO_ADC			3u

	/* VARIABLES: */
	uint8_t rauw_digito[10] = {0x7E, 0x0C, 0xB6, 0x9E, 0xCC, 0xDA, 0xFA, 0x0E, 0xFE, 0xDE};

	uint8_t	rub_posUnidades = 0u;
	uint8_t	rub_posDecenas = 0u;
	uint8_t	rub_posCentenas = 0u;
	uint8_t	rub_Modo = 1u, rub_pressed = false;
	volatile uint8_t rub_GPIOBstatus = 0u;
	volatile uint16_t lub_vecesPressed = 0u;
/* This is where general variables & macros section ends */
/* This is where TIMER-SPECIFIC variables & macros definition starts */
	//	MACROS:
	#define FREQUENCYDIVIDER	4u
	#define BOARD_TPM 			TPM2
	#define BOARD_TPM_IRQ_NUM 	TPM2_IRQn				/* Interrupt number and interrupt handler for the TPM instance used */
	#define BOARD_TPM_HANDLER 	TPM2_IRQHandler
	#define TPM_SOURCE_CLOCK 	(CLOCK_GetFreq(kCLOCK_PllFllSelClk)/4)		/* Get source clock for TPM driver */
	#define LOOP				2u
	#define SECONDLOOP			50U

	//	VARIABLES:
	uint32_t cnt = 0u;
	tpm_config_t tpmInfo;

	volatile bool tpmIsrFlag = false;
	volatile uint32_t milisecondCounts = 0U;
/* This is where TIMER-SPECIFIC variables & macros definition ends */
/* This is where ADC-SPECIFIC variables & macros definition starts */
	//	MACROS:
	#define DEMO_ADC16_BASE 			ADC0
	#define DEMO_ADC16_CHANNEL_GROUP 	0U
	#define DEMO_ADC16_USER_CHANNEL 	0U 		/*PTE20, ADC0_SE0 */

	#define DEMO_ADC16_IRQn 				ADC0_IRQn
	#define DEMO_ADC16_IRQ_HANDLER_FUNC 	ADC0_IRQHandler

	#define K	0.05f

	// VARIABLES:
	volatile bool g_Adc16ConversionDoneFlag = false;
	volatile uint32_t g_Adc16ConversionValue;
	uint32_t rul_ADCVoltage = 0;
	adc16_channel_config_t 	adc16ChannelConfigStruct;
/* This is where ADC-SPECIFIC variables & macros definition ends */

/* TODO: FUNCTION PROTOTYPES SECTION */
	void MAIN_P01_delay(volatile uint32_t lul_DelayTime);

	void MAIN_P01_WriteDigitOnPTx(GPIO_Type *base, uint16_t rbcdDigit);

	void MAIN_P02_timerConfiguration(void);
	void MAIN_P02_ADCConfiguration(void);

	void MAIN_P01_Set_GPIO(GPIO_Type *base, uint32_t mask, uint8_t direction);

	void MAIN_P01_MultiplexarYEscribir(uint32_t vecesMultiplex);

	int MAIN_P01_CheckForGPIOx(GPIO_Type *base, uint32_t lul_bitsToSet);

	bool MAIN_P02_incrementPressed(void);
	bool MAIN_P02_decrementPressed(void);
	bool MAIN_P02_resetPressed(void);

	void MAIN_P02_MODO01(void);
	void MAIN_P02_MODO02(void);
	void MAIN_P02_MODO03(void);

	void MAIN_P02_MODECHANGER(void);

	static void MAIN_P02_incrementHandler(void);
	static void MAIN_P02_decrementHandler(void);

	void DEMO_ADC16_IRQ_HANDLER_FUNC(void);

	void MAIN_P02_SetADContoDisplay(void);
/*
 * @brief   Application entry point.
 */
int main(void){
	/* Init board hardware. */
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	BOARD_InitPins();

	MAIN_P02_ADCConfiguration();
	MAIN_P02_timerConfiguration();
	EnableIRQ(DEMO_ADC16_IRQn);

	/* Init GPIOC 7:0 as outputs */
	MAIN_P01_Set_GPIO(GPIOC, GPIOC_OUTPUTBITS, DigitalOutput);
	/* Init GPIOE 3:0 as output */
	MAIN_P01_Set_GPIO(GPIOE, GPIOE_OUTPUTBITS, DigitalOutput);
	/* Init GPIOA 2:0 as inputs */
	MAIN_P01_Set_GPIO(GPIOB, GPIOB_INPUTBITS, DigitalInput);
	/* Init GPIOE20 as input */
	MAIN_P01_Set_GPIO(GPIOE, BIT20, DigitalInput);

	while(8){
		switch(rub_Modo){
		case CONTADOR_MANUAL:
			MAIN_P02_MODO01();
			continue;

		case CONTADOR_AUTO:
			MAIN_P02_MODO02();
			continue;

		case MODO_ADC:
			MAIN_P02_MODO03();
			continue;

		default:
			MAIN_P02_MODO01();
			continue;
		}
	}
	return 0;
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief		NO OPERATION for a set number of time
* @param		lul_DelayTime	: Saves the time (in milleseconds) that the program shall stop executing
* @retval		void
*/
void MAIN_P01_delay(volatile uint32_t lul_DelayTime){
	volatile uint32_t n = 0;
	lul_DelayTime *= 3690;

    for (n = 0; n < lul_DelayTime; n++)
    {
        __asm("NOP"); 				/* delay */
    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief			Use this function to set a GPIO a input/output.
* @param	base	GPIO peripheral base pointer(PTA, PTB, PTC, etc.)
* @param	mask	Hex-coded value to load onto base
* @retval 			void
*/
void inline MAIN_P01_WriteDigitOnPTx(GPIO_Type *base, uint16_t bcdDigit){
	base->PDOR = bcdDigit;
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief			Use this function to set a GPIO a input/output.
* @param	base	GPIO peripheral base pointer(PTA, PTB, PTC, etc.)
* @param	mask	Hex-coded value to set on GPIOx, config.pinDirection will only take effect on those bits set as 1 on mask
* @param	config	Structure used to set the bits specified on mask as input/output, this funcion ignores config.outputLogic
* @retval 			void
*/
 void MAIN_P01_Set_GPIO(GPIO_Type *base, uint32_t mask, uint8_t direction){
	if(direction == DigitalInput)
		base->PDDR &= ~(mask);
	else
	    {
			base->PDDR |= mask;
			base->PCOR |= mask;
	    }
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief	Use this function to multiplex and write onto a 7 segments x4 display
* @param	vecesMultiplex	Is the number of times the function will be looped(generates a 4xvecesMultiplex delay)
* @retval 	void
*/
void MAIN_P01_MultiplexarYEscribir(uint32_t vecesMultiplex){
	volatile uint16_t luw_delayMultiplex = 1u;
	while(vecesMultiplex)
	{
		GPIOC->PDOR = rauw_digito[rub_posUnidades];
		GPIOE->PDOR |= BIT0;
		MAIN_P01_delay(luw_delayMultiplex);
		GPIOE->PDOR ^= BIT0;

		GPIOC->PDOR = rauw_digito[rub_posDecenas];
		GPIOE->PDOR |= BIT1;
		MAIN_P01_delay(luw_delayMultiplex);
		GPIOE->PDOR ^= BIT1;

		GPIOC->PDOR = rauw_digito[rub_posCentenas];
		if(rub_Modo == 3u)
			GPIOC->PSOR |= DECIMAL_POINT;
		GPIOE->PDOR |= BIT2;
		MAIN_P01_delay(luw_delayMultiplex);
		GPIOE->PDOR ^= BIT2;

		GPIOC->PDOR = rauw_digito[rub_Modo];
		GPIOE->PDOR |= BIT3;
		MAIN_P01_delay(luw_delayMultiplex);
		GPIOE->PDOR ^= BIT3;

		vecesMultiplex--;
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief		Reads the value of a GPIO
* @param		base		: GPIO to be read
* @param		bitsToSet	: Masks the bits that shall be read
* @retval		Masked value of the GPIO
*/
int inline MAIN_P01_CheckForGPIOx(GPIO_Type *base, uint32_t lul_bitsToSet){
	return (~base->PDIR) & lul_bitsToSet;
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief		Timer setup
* @param		void
* @retval		void
*/
inline void MAIN_P02_timerConfiguration(void){
	/* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
	CLOCK_SetTpmClock(1u);
	TPM_GetDefaultConfig(&tpmInfo);

	/* TPM clock divide by 4 */
	tpmInfo.prescale = kTPM_Prescale_Divide_4;

	/* Initialize TPM module */
	TPM_Init(BOARD_TPM, &tpmInfo);

	/* Set timer period.*/
	TPM_SetTimerPeriod(BOARD_TPM, USEC_TO_COUNT(1000U, TPM_SOURCE_CLOCK));

	TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);
	EnableIRQ(BOARD_TPM_IRQ_NUM);
	TPM_StartTimer(BOARD_TPM, kTPM_SystemClock);
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief		Timer  interrupt handler
* @param		void
* @retval		void
*/
void BOARD_TPM_HANDLER(void){
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
    tpmIsrFlag = true;
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief		Function to be executed on Mode01 setup
* @param		void
* @retval		void
*/

void MAIN_P02_MODO01(void){
		rub_GPIOBstatus = MAIN_P01_CheckForGPIOx(GPIOB, THREE_BUTTON_BITS);

		if(rub_GPIOBstatus)
			rub_pressed = true;

		if(MAIN_P02_incrementPressed())
			{
				MAIN_P02_incrementHandler();
			}

		else if(MAIN_P02_decrementPressed())
			{
				MAIN_P02_decrementHandler();
			}

		MAIN_P01_MultiplexarYEscribir(CONST_VECES);
		MAIN_P02_MODECHANGER();
}

/*---------------------------------------------------------------------------------------------------------*/
/*
* @brief		Function to be executed on Mode02 setup
* @param		void
* @retval		void
*/
void MAIN_P02_MODO02(void){
		if(tpmIsrFlag)
		{

			tpmIsrFlag = false;
			milisecondCounts++;

			if(milisecondCounts >= SECONDLOOP)
			{
				MAIN_P02_incrementHandler();

				if(cnt >= LOOP)
				{
					cnt = 0;

				}
				milisecondCounts = 0U;

			}
		}
		__WFI();
		MAIN_P02_MODECHANGER();
		MAIN_P01_MultiplexarYEscribir(CONST_VECES);
}
/*
* @brief		Function to be executed on Mode03 setup
* @param		void
* @retval		void
*/
void MAIN_P02_MODO03(void){
	MAIN_P02_SetADContoDisplay();
	MAIN_P02_MODECHANGER();
	MAIN_P01_MultiplexarYEscribir(CONST_VECES);
}

/*
* @brief		Manages the change of mode and reset function
* @param		void
* @retval		void
*/
void MAIN_P02_MODECHANGER(void){
	bool lb_resetPressed = false;
	lub_vecesPressed = 0u;

	rub_GPIOBstatus = MAIN_P01_CheckForGPIOx(GPIOB, THREE_BUTTON_BITS);

	if(MAIN_P02_resetPressed())
		{
			rub_pressed = true;
			lb_resetPressed = true;
		}

	while(rub_pressed)
		{
			if(rub_pressed){
				lub_vecesPressed++;
				if(rub_Modo == MODO_ADC)
					MAIN_P02_SetADContoDisplay();
			}

			MAIN_P01_MultiplexarYEscribir(CONST_VECES);

			rub_GPIOBstatus = MAIN_P01_CheckForGPIOx(GPIOB, THREE_BUTTON_BITS);

			if(!rub_GPIOBstatus){
				rub_pressed = false;
				lub_vecesPressed = 0u;

				if(lb_resetPressed)
					{
						if(rub_Modo != MODO_ADC)
						{
							rub_posUnidades = 0;
							rub_posDecenas = 0;
							rub_posCentenas = 0;
						}
						lb_resetPressed = false;
					}
			}

			if((lub_vecesPressed > CYCLES_CHANGE) && MAIN_P02_resetPressed())
				{
					if(rub_Modo != MODO_ADC)
						rub_Modo++;
					else
						rub_Modo = 1u;
					if(rub_Modo != MODO_ADC)
					{
						rub_posUnidades = 0;
						rub_posDecenas = 0;
						rub_posCentenas = 0;
					}
					break;
				}
			}
}

/*
* @brief		Handles the increment of each digit
* @param		void
* @retval		void
*/
static void MAIN_P02_incrementHandler(void){
	rub_posUnidades++;
	if(rub_posUnidades == 10)
		{
			rub_posUnidades = 0;
			rub_posDecenas++;
			if(rub_posDecenas == 10)
				{
					rub_posDecenas = 0;
					rub_posCentenas++;
					if(rub_posCentenas == 10)
						rub_posCentenas = 0;
				}
		}
}

/*
* @brief		Handles the decrement of each digit
* @param		void
* @retval		void
*/
static void MAIN_P02_decrementHandler(void){
	if(rub_posUnidades == 0)
		{
			rub_posUnidades = 9;
			if(rub_posDecenas == 0)
				{
					rub_posDecenas = 9;
					if(rub_posCentenas == 0)
						rub_posCentenas = 9;
					else rub_posCentenas--;
				}
		else rub_posDecenas--;
		}
	else rub_posUnidades--;
}

void DEMO_ADC16_IRQ_HANDLER_FUNC(void)
{
    g_Adc16ConversionDoneFlag = true;
    g_Adc16ConversionValue = ADC16_GetChannelConversionValue(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP);
}

void MAIN_P02_ADCConfiguration(void){
	adc16_config_t 			adc16ConfigStruct;
	ADC16_GetDefaultConfig(&adc16ConfigStruct);
	adc16ConfigStruct.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;

	ADC16_Init(DEMO_ADC16_BASE, &adc16ConfigStruct);
	ADC16_EnableHardwareTrigger(DEMO_ADC16_BASE, false); 				/* SOFTWARE TRIGGER */

	/* FSL_FEATURE_ADC16_HAS_CALIBRATION */
	adc16ChannelConfigStruct.channelNumber = DEMO_ADC16_USER_CHANNEL;
	adc16ChannelConfigStruct.enableInterruptOnConversionCompleted = true; /* Enable the interrupt. */
	adc16ChannelConfigStruct.enableDifferentialConversion = false;
}

void MAIN_P02_SetADContoDisplay(void){
	ADC16_SetChannelConfig(DEMO_ADC16_BASE, DEMO_ADC16_CHANNEL_GROUP, &adc16ChannelConfigStruct);
	while(!g_Adc16ConversionDoneFlag);

	rul_ADCVoltage = g_Adc16ConversionValue*3000/TWOpower16;

    static uint16_t memory;
    memory = memory*(1-K) + rul_ADCVoltage*K;
    rul_ADCVoltage = memory;

	rub_posCentenas = rul_ADCVoltage/1000;
	rub_posDecenas = (rul_ADCVoltage%1000)/100;
	rub_posUnidades = (rul_ADCVoltage%100)/10;
}

bool MAIN_P02_incrementPressed(void){
	return(rub_GPIOBstatus & 0x1u);
}

bool MAIN_P02_decrementPressed(void){
	return(rub_GPIOBstatus & 0x2u);
}

bool MAIN_P02_resetPressed(void){
	return(rub_GPIOBstatus & 0x4u);
}
