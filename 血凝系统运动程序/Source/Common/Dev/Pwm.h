/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Pwm.h
 *******************************************************************/
#ifndef _PWM_H_
#define _PWM_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
/********************************************************************
 *
 *******************************************************************/
VOID PwmInit(VOID);
VOID PwmStart(VOID);
VOID PwmStop(VOID);
VOID PwmSetDutyRatio(UINT32 ratioX1000);
/********************************************************************
 *
 *******************************************************************/

#endif // _PWM_H_

