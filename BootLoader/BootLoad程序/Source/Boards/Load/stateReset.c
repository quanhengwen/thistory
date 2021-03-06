/********************************************************************
 * Shenzhen Thistory Bio. Tech. Co.
 * Copyright 2018 - 2018
 *
 * stateReset.c
 *******************************************************************/
#include "Load.h"
#include "Console.h"
#include "SlideState.h"
/********************************************************************
 *
 *******************************************************************/
#define LEAVE_OPTO_STEPS_DISP    (1600)
#define LEAVE_OPTO_STEPS_LOAD    (-3200)

#define MEET_OPTO_STEPS_DISP_MAX (3200)
#define MEET_OPTO_STEPS_LOAD_MAX (12000)

#define RESET_OPTO_ATPOS_DISP	 (1)
#define RESET_OPTO_ATPOS_LOAD    (0)

#define RESET_SPEED	(0)
/********************************************************************
 *
 *******************************************************************/
STATIC CONST INT32 leaveOptoSteps[MOTOR_MAX] = {
	LEAVE_OPTO_STEPS_DISP,
	LEAVE_OPTO_STEPS_LOAD,
};

STATIC CONST INT32 MeetOptoStepsMax[MOTOR_MAX] = {
	MEET_OPTO_STEPS_DISP_MAX,
	MEET_OPTO_STEPS_LOAD_MAX,
};

/********************************************************************
 *
 *******************************************************************/
STATIC BOOL isOptoOnReset(UINT8 motorId)
{
	if (motorId == MOTOR_DISP){
		StepMotor* motor = &(loadApp.motors[MOTOR_DISP]);
		UINT16 opto = StepMotorGetOpto (motor);
		if (opto == RESET_OPTO_ATPOS_DISP) return TRUE;
		else return FALSE;
	}
	else if(motorId == MOTOR_LOAD) {
		StepMotor* motor = &(loadApp.motors[MOTOR_LOAD]);
		UINT16 opto = StepMotorGetOpto (motor);
		if (opto == RESET_OPTO_ATPOS_LOAD) return TRUE;
		else return FALSE;
	}
	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
STATIC INT32 getReadyPos(UINT8 motorId)
{
	if (motorId == MOTOR_DISP)
		return LoadGetWrkPosByHole1 (0);
	return 0;
}
/********************************************************************
 *
 *******************************************************************/

VOID MessageReset(Message* msg)
{
	if(msg){
		UINT8 motorId;
		if(msg->p1 == MSG_MOTOR_DISP)      motorId = MOTOR_DISP;
		else if(msg->p1 == MSG_MOTOR_LOAD) motorId = MOTOR_LOAD;
		else 				  return;

		loadApp.bMotorReset[motorId] = FALSE;
		loadApp.currentResetMotor    = motorId;
		loadApp.state0 = STATE0_RESET;
		StepMotor* motor = &(loadApp.motors[motorId]);

		if(isOptoOnReset (motorId)){
			loadApp.state1 = STATE1_RST_LEAVE_OPTO;
			INT32 steps = leaveOptoSteps[motorId];
			StepMotorMovRel (motor, steps, RESET_SPEED);
		}
		else{
			loadApp.state1 = STATE1_RST_MEET_OPTO;
			INT32 steps = MeetOptoStepsMax[motorId];
			if(motorId == MOTOR_DISP)
				StepMotorMovUntilOptoTwice (motor, steps, RESET_SPEED);
			else if(motorId == MOTOR_LOAD)
				StepMotorMovUntilOpto (motor, steps, RESET_SPEED);
		}
	}
}

/********************************************************************
 *
 *******************************************************************/
#define ABS(x) ((x) >=0 ? (x) : -(x))
STATIC BOOL isEnoughDistanceAsMeetOpto(StepMotor* motor)
{
	INT32 posOpto = motor->optoCond.posOpto;
	INT32 posCurr = motor->pos;

	if(ABS(posOpto-posCurr) >= DISP_QUARTER_STEPS)
		return TRUE;

	return FALSE;
}
/********************************************************************
 *
 *******************************************************************/
//STATIC VOID setOriginHere(StepMotor* motor)
//{
//	motor->pos = 0;
//}

/********************************************************************
 *
 *******************************************************************/
STATIC VOID toReadyOrToIdle(VOID)
{
	BOOL isReady = TRUE;
	for(INT32 i=0; i<MOTOR_MAX; i++)
		isReady = (isReady && loadApp.bMotorReset[i]);
	if(isReady){
		loadApp.state0 = STATE0_READY;
		loadApp.state1_Load = STATE1_RDY_LOAD_IDLE;
		loadApp.state1_Disp = STATE1_RDY_DISP_IDLE;
		loadApp.autoLoadCnt = 0;
		loadApp.bAutoLoad = FALSE;
		loadApp.whichHoleOnWork = HOLE_MIN;
		loadApp.whichWorkHoleOn = WORK_MIN;

#ifdef LOAD_TEST_AUTOLOAD
		Message msg;
		msg.id = MSG_TEST;
		MessagePost(&msg);
#endif
	}
	else{
		loadApp.state1 = STATE1_RST_IDLE;
	}
}

/********************************************************************
 *  ========= Leave Opto ===========
 *******************************************************************/
STATIC VOID MessageOnStateResetLeaveOpto(Message* msg)
{
	switch(msg->id){
	case MSG_DISP_DONE:
		if (loadApp.currentResetMotor == MOTOR_DISP){
			StepMotor* motor = &(loadApp.motors[MOTOR_DISP]);
			INT32 steps      = MeetOptoStepsMax[MOTOR_DISP];
			loadApp.state1   = STATE1_RST_GOTO_OPTO;

			StepMotorMovUntilOptoTwice (motor, steps, RESET_SPEED);
		}
		break;
	case MSG_LOAD_DONE:
		if (loadApp.currentResetMotor == MOTOR_LOAD){
			StepMotor* motor = &(loadApp.motors[MOTOR_LOAD]);
			INT32 steps      = MeetOptoStepsMax[MOTOR_LOAD];
			loadApp.state1   = STATE1_RST_GOTO_OPTO;

			StepMotorMovUntilOpto (motor, steps, RESET_SPEED);
		}
		break;
	}
}
/********************************************************************
 *  ========= Meet Opto ===========
 *******************************************************************/
STATIC VOID MessageOnStateResetMeetOpto(Message* msg)
{
	switch(msg->id){
	case MSG_DISP_DONE:
		if (loadApp.currentResetMotor == MOTOR_DISP) {
			StepMotor* motor = &(loadApp.motors[MOTOR_DISP]);
			if (isEnoughDistanceAsMeetOpto (motor)){
				//setOriginHere (motor);
				loadApp.state1 = STATE1_RST_GOTO_RDY;
				INT32 pos = getReadyPos (MOTOR_DISP);
				LoadStepMotorMovAbs (motor, pos, RESET_SPEED);
			}
			else{
				loadApp.state1 = STATE1_RST_LEAVE_OPTO;
				INT32 steps = leaveOptoSteps[MOTOR_DISP];
				StepMotorMovRel (motor, steps, RESET_SPEED);
			}
		}
		break;
	case MSG_LOAD_DONE:
		if (loadApp.currentResetMotor == MOTOR_LOAD) {
			StepMotor* motor = &(loadApp.motors[MOTOR_LOAD]);
			if (isEnoughDistanceAsMeetOpto (motor)){
				//setOriginHere (motor);
				loadApp.state1 = STATE1_RST_GOTO_RDY;
				INT32 pos = getReadyPos (MOTOR_LOAD);
				LoadStepMotorMovAbs (motor, pos, RESET_SPEED);
			}
			else{
				loadApp.state1 = STATE1_RST_LEAVE_OPTO;
				INT32 steps = leaveOptoSteps[MOTOR_LOAD];
				StepMotorMovRel (motor, steps, RESET_SPEED);
			}
		}
		break;
	}
}
/********************************************************************
 *  ========= Goto Opto ===========
 *******************************************************************/
STATIC VOID MessageOnStateResetGotoOpto(Message* msg)
{
	switch(msg->id){
	case MSG_DISP_DONE:
		if(loadApp.currentResetMotor == MOTOR_DISP){
			StepMotor* motor = &(loadApp.motors[MOTOR_DISP]);
			//setOriginHere (motor);
			loadApp.state1 = STATE1_RST_GOTO_RDY;
			INT32 pos = getReadyPos (MOTOR_DISP);
			LoadStepMotorMovAbs (motor, pos, RESET_SPEED);
		}
		break;
	case MSG_LOAD_DONE:
		if(loadApp.currentResetMotor == MOTOR_LOAD){
			StepMotor* motor = &(loadApp.motors[MOTOR_LOAD]);
			//setOriginHere (motor);
			loadApp.state1 = STATE1_RST_GOTO_RDY;
			INT32 pos = getReadyPos (MOTOR_LOAD);
			LoadStepMotorMovAbs (motor, pos, RESET_SPEED);
		}
		break;
	}
}
/********************************************************************
 *  ========= Goto Ready ===========
 *******************************************************************/
STATIC VOID MessageOnStateResetGotoReady(Message* msg)
{
	switch(msg->id){
	case MSG_DISP_DONE:
		if(loadApp.currentResetMotor == MOTOR_DISP){
			loadApp.bMotorReset[MOTOR_DISP] = TRUE;
			toReadyOrToIdle();

			SlideStateWatchStop ();
			SlideStateUpdateAsDispatched ();
			if (loadApp.needResponse){
				loadApp.needResponse = FALSE;
				ResponseSuccess();
			}
		}
		break;
	case MSG_LOAD_DONE:
		if(loadApp.currentResetMotor == MOTOR_LOAD){
			loadApp.bMotorReset[MOTOR_LOAD] = TRUE;
			toReadyOrToIdle();

			SlideStateWatchStop ();
			SlideStateUpdateAsLoaded ();
			if (loadApp.needResponse){
				loadApp.needResponse = FALSE;
				ResponseSuccess();
			}
		}
		break;
	}
}

/********************************************************************
 *  ========== IDLE ===========
 *******************************************************************/

STATIC VOID MessageOnStateResetIdle(Message* msg)
{
	switch(msg->id){
	case MSG_HOST_MOTOR_ENABLE:
	case MSG_HOST_MOTOR_TOZERO:
	case MSG_HOST_MOTOR_MOVREL:
	case MSG_HOST_MOTOR_MOVABS:
	case MSG_HOST_MOTOR_MOVWRK:
	case MSG_HOST_MOTOR_GETPOS:
		LoadBasicCommandOnStateAndBackTo (STATE0_RESET, msg);
		break;
	case MSG_CMD_MOVREL:
	case MSG_CMD_MOVABS:
		LoadBasicCommandOnStateAndBackTo (STATE0_RESET, msg);
		break;
	case MSG_HOST_MOTOR_RESET:
		loadApp.needResponse = TRUE;
		// go through
	case MSG_CMD_RESET:
		SlideStateWatchStart ();
		MessageReset(msg);
		break;
	case MSG_HOST_LOADCUP:
	case MSG_HOST_DISPATCH:
	case MSG_CMD_DISP:
	case MSG_CMD_LOAD:
		// some error found!
		break;
	default:
		break;
	}
}
/********************************************************************
 *   ========== RESET ===========
 *******************************************************************/

VOID MessageOnStateReset(Message* msg)
{
	switch(loadApp.state1){
	case STATE1_RST_IDLE:
		MessageOnStateResetIdle (msg);
		break;
	case STATE1_RST_LEAVE_OPTO:
		MessageOnStateResetLeaveOpto (msg);
		break;
	case STATE1_RST_MEET_OPTO:
		MessageOnStateResetMeetOpto (msg);
		break;
	case STATE1_RST_GOTO_OPTO:
		MessageOnStateResetGotoOpto (msg);
		break;
	case STATE1_RST_GOTO_RDY:
		MessageOnStateResetGotoReady (msg);
		break;
	}
}

/********************************************************************
 *
 *******************************************************************/



