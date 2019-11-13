/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Board.c
 *******************************************************************/

#include "Board.h"
/********************************************************************
 *
 *******************************************************************/
typedef struct __Board{
	Uart* uartConsole;
	Uart* uartHost;
	Gpio  gpios[IO_MAX];
	Spi*  spiFRam;
	Spi*  spiExt;
	Pulse* pulseDispatch;
	Pulse* pulseLoad;
}Board;

STATIC Board board;
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetConsoleUart(VOID)
{
	return board.uartConsole;
}
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetHostUart(VOID)
{
	return board.uartHost;
}
/********************************************************************
 *
 *******************************************************************/
Gpio* BoardGetGpio(UINT16 id)
{
	if (id < IO_MAX)
		return &(board.gpios[id]);
	return NULL;
}
/********************************************************************
 *
 *******************************************************************/
Spi*  BoardGetSpiFRam(VOID)
{
	return board.spiFRam;
}
/********************************************************************
 *
 *******************************************************************/
Spi*  BoardGetSpiExt(VOID)
{
	return board.spiExt;
}
/********************************************************************
 *
 *******************************************************************/
Pulse*  BoardGetPulseDispatch(VOID)
{
	return board.pulseDispatch;
}
/********************************************************************
 *
 *******************************************************************/
Pulse*  BoardGetPulseLoad(VOID)
{
	return board.pulseLoad;
}

/********************************************************************
 * Rcc Init
 *******************************************************************/
STATIC VOID RccInit(VOID)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
#if 1
	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 			= RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue 	= RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL 		= RCC_PLL_MUL6;
#else
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
#endif
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                             	 |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
/********************************************************************
 *
 *******************************************************************/
STATIC VOID BoardGpioInit(VOID)
{
	board.gpios[IO_LPD1].io  = GPIOF;
	board.gpios[IO_LPD1].pin = GPIO_PIN_14;
	GpioInit(&(board.gpios[IO_LPD1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_LPD2].io  = GPIOE;
	board.gpios[IO_LPD2].pin = GPIO_PIN_8;
	GpioInit(&(board.gpios[IO_LPD2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_ORI1].io  = GPIOF;
	board.gpios[IO_ORI1].pin = GPIO_PIN_15;
	GpioInit(&(board.gpios[IO_ORI1]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_ORI2].io  = GPIOE;
	board.gpios[IO_ORI2].pin = GPIO_PIN_9;
	GpioInit(&(board.gpios[IO_ORI2]),
		GPIO_MODE_OUTPUT_PP, GPIO_SPEED_FREQ_LOW);

	/************************************************/

	board.gpios[IO_OPTO1].io  = GPIOF;
	board.gpios[IO_OPTO1].pin = GPIO_PIN_0;
	GpioInit(&(board.gpios[IO_OPTO1]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO2].io  = GPIOF;
	board.gpios[IO_OPTO2].pin = GPIO_PIN_1;
	GpioInit(&(board.gpios[IO_OPTO2]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO3].io  = GPIOF;
	board.gpios[IO_OPTO3].pin = GPIO_PIN_2;
	GpioInit(&(board.gpios[IO_OPTO3]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO4].io  = GPIOF;
	board.gpios[IO_OPTO4].pin = GPIO_PIN_3;
	GpioInit(&(board.gpios[IO_OPTO4]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO5].io  = GPIOF;
	board.gpios[IO_OPTO5].pin = GPIO_PIN_4;
	GpioInit(&(board.gpios[IO_OPTO5]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);

	board.gpios[IO_OPTO6].io  = GPIOF;
	board.gpios[IO_OPTO6].pin = GPIO_PIN_5;
	GpioInit(&(board.gpios[IO_OPTO6]),
		GPIO_MODE_INPUT, GPIO_SPEED_FREQ_LOW);
}
/********************************************************************
 *
 *******************************************************************/
VOID  BoardInit(VOID)
{
	HAL_Init();
	RccInit();

	BoardGpioInit ();
	board.uartHost    = UartGetObject (UART01);
	board.uartConsole = UartGetObject (UART03);
	board.spiFRam     = SpiGetObject (SPI02);
	board.spiExt      = SpiGetObject (SPI01);
	board.pulseDispatch = PulseGetObject (PULSE01);
	board.pulseLoad     = PulseGetObject (PULSE02);
}
