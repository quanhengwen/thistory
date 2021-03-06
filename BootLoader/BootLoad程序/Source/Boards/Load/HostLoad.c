/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * HostLoad.c
 *******************************************************************/
#include "HostLoad.h"
#include "Host.h"
#include "Console.h"
#include "Message.h"
#include "Load.h"
#include "SerialDownLoad.h"
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdVersion(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		msg.id = MSG_HOST_VERSION;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdGetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdSetInfo(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *宏定义
 *******************************************************************/
STATIC_BOOL_IAP_HOST_CMD_UPDATE_ASK_FUNCTION();
STATIC_BOOL_IAP_HOST_CMD_UPDATE_TRAN_FUNCTION();
/********************************************************************
 *参数信息查询
 *******************************************************************/
STATIC BOOL HostCmdParaInfo(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		msg.id = MSG_HOST_PARINFO;
		INT16 id1 = frame->data[0]*256 + frame->data[1];
		INT16 id2 = frame->data[2]*256 + frame->data[3];
		INT16 id3 = frame->data[4]*256 + frame->data[5];
		INT16 id4 = frame->data[6]*256 + frame->data[7];
		msg.p1 = id1;
		msg.p2 = id2;
		msg.p3 = id3;
		msg.p4 = id4;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *参数信息设置
 *******************************************************************/
STATIC BOOL HostCmdParaSet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 8){
		msg.id = MSG_HOST_PARSET;
		UINT16 id1 = frame->data[0]*256 + frame->data[1];
		UINT16 id2 = frame->data[2]*256 + frame->data[3];
		INT16 hword1 = MAKE_WORD (frame->data[4], frame->data[5]);
		INT16 lword1 = MAKE_WORD (frame->data[6], frame->data[7]);
		INT32 value1 = MAKE_DWORD (hword1, lword1);
		INT16 hword2 = MAKE_WORD (frame->data[8], frame->data[9]);
		INT16 lword2 = MAKE_WORD (frame->data[10], frame->data[11]);
		INT32 value2 = MAKE_DWORD (hword2, lword2);
		msg.p1 = id1;
		msg.p2 = id2;
		msg.p3 = value1;
		msg.p4 = value2;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *工作位置保存
 *******************************************************************/
STATIC BOOL HostCmdWorkSave(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		msg.id = MSG_HOST_WRKSAVE;
		INT32 work = frame->data[0] * 256 + frame->data[1];
		msg.p1 = work;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMotorEnable(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMotorReset(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 motor = frame->data[0];
		UINT8 speed = frame->data[1];
		msg.id = MSG_HOST_MOTOR_RESET;
		msg.p1 = motor;
		msg.p2 = speed;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMotorToZero(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *电机相对运动
 *******************************************************************/
STATIC BOOL HostCmdMotorMovRel(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 4){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT32 pluse = frame->data[2] * 256 + frame->data[3];
		UINT8 dir = frame->data[4];
		msg.id = MSG_HOST_MOTOR_MOVREL;			
		msg.p1 = motor_code;
		msg.p2 = speed ;
		msg.p3 = pluse ;
		msg.p4 = dir ;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机运动到指定微步
 *******************************************************************/
STATIC BOOL HostCmdMotorMovAbs(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 3){
		UINT8 motor_code  = frame->data[0];
		UINT8 speed = frame->data[1];
		UINT32 pluse = frame->data[2] * 256 + frame->data[3];
		msg.id = MSG_HOST_MOTOR_MOVABS;
		msg.p1 = motor_code;
		msg.p2 = speed ;
		msg.p3 = pluse ;
		msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *电机运动到指定工作位置
 *******************************************************************/
STATIC BOOL HostCmdMotorMovWrk(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdMotorGetPos(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdLoadCup(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 wrkpos = frame->data[0];
		msg.id = MSG_HOST_LOADCUP;
		msg.p1 = wrkpos;
		msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdDispatch(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 2){
		UINT8 hole = frame->data[0];
		UINT8 work = frame->data[1];
		msg.id = MSG_HOST_DISPATCH;
		msg.p1 = hole;
		msg.p2 = work;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;

}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdIoSet(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdIoGet(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdOptoGet(HostFrame* frame)
{
	Message msg;
	if ( frame && frame->dataLen >= 1){
		UINT8 num = frame->data[0];
		msg.id = MSG_HOST_OPTOGET;
		msg.p1 = num;
		MessagePost(&msg);
		return TRUE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC BOOL HostCmdFault(HostFrame* frame)
{
	return TRUE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC UINT32 HostLoadParser(HostFrame* frame)
{
	UINT8 tag = HostGetConsoleTag();
	BOOL ret = FALSE;
	if(!frame) return HOST_ACK_ERR_LEN;
	loadApp.receiveCmd = frame->command;
	switch(frame->command){
	case HOST_CMD_VERSION       : ret = HostCmdVersion (frame);     break;
	case HOST_CMD_GETINFO       : ret = HostCmdGetInfo (frame);     break;
	case HOST_CMD_SETINFO       : ret = HostCmdSetInfo (frame);     break;
	case HOST_CMD_PARAINFO      : ret = HostCmdParaInfo (frame);    break;
	case HOST_CMD_PARASET		: ret = HostCmdParaSet(frame);  	break;
	case HOST_CMD_WRKSAVE       : ret = HostCmdWorkSave(frame); 	break;		
	case HOST_CMD_MOTOR_ENABLE	: ret = HostCmdMotorEnable (frame); break;
	case HOST_CMD_MOTOR_RESET	: ret = HostCmdMotorReset  (frame); break;
	case HOST_CMD_MOTOR_TOZERO	: ret = HostCmdMotorToZero (frame); break;
	case HOST_CMD_MOTOR_MOVREL	: ret = HostCmdMotorMovRel (frame); break;
	case HOST_CMD_MOTOR_MOVABS	: ret = HostCmdMotorMovAbs (frame); break;
	case HOST_CMD_MOTOR_MOVWRK	: ret = HostCmdMotorMovWrk (frame); break;
	case HOST_CMD_MOTOR_GETPOS	: ret = HostCmdMotorGetPos (frame); break;
	case HOST_CMD_LOADCUP	    : ret = HostCmdLoadCup (frame);     break;
	case HOST_CMD_DISPATCH	    : ret = HostCmdDispatch (frame);    break;
	case HOST_CMD_IOSET		    : ret = HostCmdIoSet (frame);       break;
	case HOST_CMD_IOGET		    : ret = HostCmdIoGet (frame);       break;
	case HOST_CMD_OPTOGET	    : ret = HostCmdOptoGet (frame);     break;		
	CASE_HOST_CMD_IAP_TRAN;
	CASE_HOST_CMD_IAP_ASK;
	case HOST_CMD_FAULT	        : ret = HostCmdFault (frame);       break;
	default:
		DebugWarning (tag, "Found Unknown Command!");
		return (HOST_ACK_ERR_ADDR);;
	}
	if(ret){
		return (HOST_ACK_SUCCESS);
	}else{
		DebugWarning(tag, "Found Length Error!");
		return (HOST_ACK_ERR_LEN);
	}
}
/********************************************************************
 *
 *******************************************************************/
VOID HostLoadInit(VOID)
{
	HostParserRegister (HOST_ADDR_LOAD, HostLoadParser);
}
/********************************************************************
 * End of File
 *******************************************************************/



