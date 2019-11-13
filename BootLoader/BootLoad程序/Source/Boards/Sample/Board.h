/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Board.h
 *******************************************************************/
#ifndef _BOARD_H_
#define _BOARD_H_
/********************************************************************
 * Macro defined
 *******************************************************************/
#include "Datatypes.h"
#include "Uart.h"
#include "Uart4Axis.h"
#include "Gpio.h"
#include "Spi.h"
#include "Pulse.h"
#include "LevelDetection.h"
#include "ConfigSample.h"
/********************************************************************
 *
 *******************************************************************/
#define PULSE_NVIC_PRIOR (1)
#define TIMER_NVIC_PRIOR (2)
#define UART_NVIC_PRIOR	 (3)
/********************************************************************
 *
 *******************************************************************/
#define EN_CONSOLE
#define EN_UART01
// #define EN_UART02
#define EN_UART03
// #define EN_UART04
// #define EN_UART05
#define EN_PULSE01_IRQ
#define EN_PULSE02_IRQ
#define EN_PULSE03_IRQ
#define EN_PULSE04_IRQ

enum{
	MSG_UNKNOWN = 0, // 0 for unknown msg
	MSG_TIMER,
	MSG_TIMER2,
	MSG_X_DONE,
	MSG_Y_DONE,
	MSG_Z_DONE,
	MSG_SYN_DONE,
	MSG_TIR_DONE,
	MSG_DET_DONE,
	MSG_COLL_DONE,
	MSG_CFG_RESET,
	MSG_HOST_VERSION = 20,
	MSG_HOST_GETINFO,
	MSG_HOST_SETINFO,
	MSG_HOST_PARINFO,
	MSG_HOST_PARSET,
	MSG_HOST_WRKSAVE,
	MSG_HOST_MOTOR_ENABLE,
	MSG_HOST_MOTOR_RESET,
	MSG_HOST_MOTOR_TOZERO,
	MSG_HOST_MOTOR_MOVREL,
	MSG_HOST_MOTOR_MOVABS,
	MSG_HOST_MOTOR_MOVWRK,
	MSG_HOST_MOTOR_ZAIXDET,
	MSG_HOST_MOTOR_SUBDRA,
	MSG_HOST_MOTOR_INQUIRE,
	MSG_HOST_MOTOR_MOVXY,
	MSG_REGEHOST_MOTOR_ZAIXDET,
	MSG_HOST_MOTOR_ZAXISSUBBUF,
	MSG_HOST_MOTOR_ZAXISSUB,
	MSG_HOST_MOTOR_ZAXISDRA,
	MSG_HOST_MOTOR_ZAXISDRABUF,
	MSG_HOST_MOTOR_ZAXISCLE,
	MSG_HOST_MOTOR_PUMPSET,
	MSG_HOST_MOTOR_PUMPGET,
	MSG_HOST_MOTOR_VALVESET,
	MSG_HOST_MOTOR_VALVEGET,
	MSG_HOST_MOTOR_IOSET,
	MSG_HOST_MOTOR_IOGET,
	MSG_HOST_MOTOR_OPTOGET,
	MSG_HOST_MOTOR_FAULT,
	MSG_DELAY,
	MSG_STOP,
	MSG_MAX,
};


enum{
	IO_LPD1 = 0,
	IO_LPD2,  IO_LPD3,  IO_LPD4,
	IO_ORI1,  IO_ORI2,  IO_ORI3,  IO_ORI4,
	IO_OPTO1, IO_OPTO2, IO_OPTO3, IO_OPTO4,
	IO_PUMP1, IO_PUMP2, IO_VALVE1,IO_LIQUID_COLL,
	IO_LIQUIDE_OUTPUT,IO_LIQUID_INPUT,IO_MAX,	
};

#define GPIO_ON           (1)
#define GPIO_OFF          (0)

enum{ MOTOR_X = 0, MOTOR_Y, MOTOR_Z, MOTOR_SYN, MOTOR_MAX};
/********************************************************************
 *
 *******************************************************************/
Uart* BoardGetConsoleUart(VOID);
Uart* BoardGetHostUart(VOID);
Gpio* BoardGetGpio(UINT16 id);
Spi*  BoardGetSpiFRam(VOID);
Spi*  BoardGetSpiExt(VOID);
Pulse*  BoardGetPulse(UINT16 motor_id);

VOID  BoardInit(VOID);
/********************************************************************
 * End of File
 *******************************************************************/
#endif // _BOARD_H_
