/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * Heat.h
 *******************************************************************/
#ifndef _HEAT_H_
#define _HEAT_H_
/********************************************************************
 *
 *******************************************************************/
#include "Datatypes.h"
/********************************************************************
 *
 *******************************************************************/
enum{
	MSG_KPSET = 100,
	MSG_KISET,
	MSG_KDSET,
	MSG_KDISP,
	MSG_STOPLOG,
	MSG_HELP,
	MSG_RUN,
	MSG_BREAK,
    
    MSG_ENABLE_HEAT,
    MSG_ASK_TEMPERATURE
};
VOID HeatInit(VOID);
#define VERSION 0x010003
/********************************************************************
 * End of File
 *******************************************************************/
#endif // 


