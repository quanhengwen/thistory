#include "stateReady.h"
/********************************************************************
 *开启液面检测
 *******************************************************************/
VOID OpenLiquid(VOID)
{
	GpioSet (BoardGetGpio(IO_LIQUIDE_OUTPUT), 0);  //开启液面检测
	LiquidneedleEnable();
}
/********************************************************************
 *关闭液面检测
 *******************************************************************/
VOID CloseLiquid(VOID)
{      
	GpioSet (BoardGetGpio(IO_LIQUIDE_OUTPUT), 1);  //关闭液面检测
	LiquidneedleDisable();
}

/********************************************************************
 *
 *******************************************************************/
VOID StepMotorSubAirCallback(VOID)
{
	Message msg;
	INT32 movpos = 0;
	movpos = GetMotorMovPos(&(MainState.motors[MOTOR_Z]));	//上升15mm以后触发一个事件	
	if(movpos == 736){
		msg.id = MSG_TIR_DONE;
		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
		MessagePost(&msg);
	}		
	if(movpos == -736){   //下降10mm后开启
		if(MainState.Parameter.liquidState == 1){
			MainState.Parameter.liquidState = 0;
			OpenLiquid();     //开启液面检测		
		}
	}
//	if(GpioGet(BoardGetGpio[IO_LIQUID_COLL]) == 0)         //撞针了，直接抛出一个事件
//	{
//		msg.id = MSG_COLL_DONE;
//		msg.p1 = msg.p2 = msg.p3 = msg.p4 = 0;
//		MessagePost(&msg);
//	}
}
/********************************************************************
 *
 *******************************************************************/
VOID RegeSubAirPluseNum(VOID)
{
	INT32 pluse = 0;
	switch(MainState.stateManageTop)
	{
		case STATE0_SUB:{	
							switch(MainState.Parameter.subType)
							{
								case 1:{
										INT32 subair = ConfigGet(CFG_REAGENT_SUB_SUM_LIQUID) - MainState.Parameter.subVolume*10;
										pluse = (subair * ConfigGet(CFG_REAGENT_STEP_UL)) / 10 ;
									}break;
								case 2:{
										pluse = (ConfigGet(CFG_REAGENT_SUB_LATER_AIR) * ConfigGet(CFG_REAGENT_STEP_UL)) / 10 ;
									}break;
								default:break;
							}							
							pluse = pluse * (-1);							
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAIN_SPEED);
					   }break;
		case STATE0_DRA:{   								
							pluse = (ConfigGet(CFG_REAGENT_DRAIN_LATER_AIR) * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;
							pluse = pluse * (-1);					
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAIN_SPEED);
					   }break;
		case STATE0_CLN:{								
							pluse = (ConfigGet(CFG_REAGENT_CLEAN_LATER_AIR) * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;	
							pluse = pluse * (-1);						
							StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), pluse, REAGENT_INJDRAINCLE_SPEED);
					   }break;
		default:break;
	}
}
/********************************************************************
 *所有电机方向统一  1为远离光耦、0为靠近光耦     注射电机则 1-吸  0-排
 *******************************************************************/
INT32 DirSortOut(INT16 motor,INT16 step,INT16 dir)
{
	switch(motor)
	{
		case MOTOR_X:{
					if(dir == MOTOR_X_POSITIVE_DIRECTION)   //1为远离光耦，作为正方向
						step = (-1) * step;
				}break;
		case MOTOR_Y:{
					if(dir == MOTOR_Y_POSITIVE_DIRECTION)   //Y轴较特殊，正方向（远离光耦）为正脉冲，负方向为负脉冲，其他轴则相反
						step = (-1) * step;
				}break;
		case MOTOR_Z:{
					if(dir == MOTOR_Z_POSITIVE_DIRECTION)   //1为远离光耦，作为正方向
						step = (-1) * step;
				}break;
		case MOTOR_SYN:{
					if(dir == MOTOR_SYN_POSITIVE_DIRECTION)   //1为吸液，作为正方向
						step = (-1) * step;
				}break;
		default:break;
	}
	return step;
}

/********************************************************************
 *中位机指令执行完后，一些状态处理及数据发送
 *******************************************************************/
VOID RegeStateEndHandler(VOID)
{
	SendResultMessage();
}
/********************************************************************
 *Basic状态,基本指令运行完后，都会进入此函数
 *******************************************************************/
VOID RegeStateBasicCmdMessageHandler(Message* msg)
{
	switch(msg->id)
	{
		case MSG_TIMER:
		case MSG_TIMER2:
		case MSG_X_DONE:
		case MSG_Y_DONE:
		case MSG_Z_DONE:
		case MSG_SYN_DONE:{
						RegeStateEndHandler();
					}break;
		default:break;
	}
}
/********************************************************************
 *Z轴检测液面
 *******************************************************************/
VOID RegeStateMotorZaixDetMessageHandler(Message* msg)
{
	switch(MainState.stateManageMid)
	{
		case STATE1_LIQUID_DET_INIT:{	
								MainState.Parameter.speed = msg->p2;
								MainState.Parameter.detState = msg->p3;							
								StepZAxisMotorMovRel (&(MainState.motors[MOTOR_Z]), -4800, MainState.Parameter.speed);   //Z轴行驶最远距离
								StepMotorSetCallback(&(MainState.motors[MOTOR_Z]), StepMotorSubAirCallback);
								MainState.stateManageMid = STATE1_LIQUID_DET_RUN;
								MainState.Parameter.liquidState = 1;   	//开启液面检测的IO引脚
								if(OPEN_SUB_HEAT)
									RegeSendCmdCtlPid(0);  //开启开环，关闭PID
							}break;
		case STATE1_LIQUID_DET_RUN:{
								switch(msg->id)
								{
									case MSG_DET_DONE:  //Z轴检测到液面												
													StepMotorBrake(&(MainState.motors[MOTOR_Z]));
													MainState.stateManageMid = STATE1_LIQUID_DET_SUCCESS;
													break;
									case MSG_Z_DONE:	//Z轴下降完成，未检测到液面																									     
													CloseLiquid();
													RegeSendCmdCtlPid(1);
													StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, MainState.Parameter.speed);//Z轴上升回原点
													MainState.stateManageMid = STATE1_LIQUID_DET_FALL;
													break;
									case MSG_COLL_DONE: //Z轴撞针
													CloseLiquid();
													RegeSendCmdCtlPid(1);
													StepMotorBrake(&(MainState.motors[MOTOR_Z]));
													MainState.stateManageMid = STATE1_LIQUID_DET_COLL;
													break;
									default:break;													
								}
							}break;
		case STATE1_LIQUID_DET_SUCCESS:{
								if(MainState.Parameter.detState){  				//检测到液面后停在该液面																		
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}									
								else{	                    //检测到液面后返回原点							
									StepZAxisMotorMovAbs (&(MainState.motors[MOTOR_Z]), 0, MainState.Parameter.speed);//Z轴上升回原点
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
								}																												
							}break;
		case STATE1_LIQUID_DET_FALL:{
								if(msg->id == MSG_Z_DONE){ //Z轴回到原点，开始切换状态																																						
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}
							}break;
		case STATE1_LIQUID_DET_COLL:{
								if(msg->id == MSG_Z_DONE) {//Z轴回到原点，开始切换状态														
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}
							}break;
		case STATE1_LIQUID_DET_MAX:{
								if((MainState.Parameter.detState == 0) && (msg->id == MSG_Z_DONE)){
									MainState.stateManageMid = STATE1_LIQUID_DET_MAX;
									RegeStateEndHandler();
								}										
							}break;
		default:break;
	}
}
/********************************************************************
 *Ready状态下处理函数
 *******************************************************************/
VOID RegeStateReadyMessageHandler(Message* msg)
{
	switch(msg->id)
	{
		case MSG_REGEHOST_MOTOR_ENABLE:{
							UINT8 motor = msg->p1 - 1;			
							StepMotorEnable(&(MainState.motors[motor]));							
							MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_RESET:{	
								MainState.stateManageTop = STATE0_RESET;
								MainState.stateManageMid = STATE1_OPT_INIT;
								RegeStateMotorResetMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_TOZERO:{
								UINT8 motor = msg->p1 - 1;
								UINT16 speed = msg->p2;
								switch(motor){
									case 0: StepMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									case 1: StepMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									case 2: StepZAxisMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									case 3: StepSYNAxisMotorMovAbs (&(MainState.motors[motor]), 0, speed); break;
									default:break;
								}								
								MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_MOVREL:{
								UINT8 motor = msg->p1 - 1;
								UINT16 speed = msg->p2;
								INT32 step = msg->p3;
								INT16 dir = msg->p4;
								step = DirSortOut(motor,step,dir);
								switch(motor){
									case 0:StepMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									case 1:StepMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									case 2:StepZAxisMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									case 3:StepSYNAxisMotorMovRel (&(MainState.motors[motor]), step, speed); break;
									default:break;
								}								
								MainState.stateManageTop = STATE0_BASIC;						
						}break;
		case MSG_REGEHOST_MOTOR_MOVABS:{
								UINT8 motor = msg->p1 - 1;
								UINT16 speed = msg->p2;
								INT32 step = msg->p3;
								switch(motor){
									case 0:StepMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									case 1:StepMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									case 2:StepZAxisMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									case 3:StepSYNAxisMotorMovAbs (&(MainState.motors[motor]), step, speed); break;
									default:break;
								}								
								MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_MOVWRK:{
								UINT8 motor = msg->p1;
								UINT16 work_position = msg->p2;
								UINT16 speed = msg->p3;			
								RegeWrkStateMessageHandler(motor, work_position, speed);
								MainState.stateManageTop = STATE0_BASIC;
						}break;
		case MSG_REGEHOST_MOTOR_ZAIXDET:{								
								MainState.stateManageTop = STATE0_ZAIXDET;
								MainState.stateManageMid = STATE1_LIQUID_DET_INIT;						
								RegeStateMotorZaixDetMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_SUBDRA:{
								INT8 dir = msg->p1;
								UINT16 speed = msg->p2;
								INT32 step =  (msg->p3 * ConfigGet(CFG_REAGENT_STEP_UL)) / 10;
								step = DirSortOut(MOTOR_SYN,step,dir);
								StepSYNAxisMotorMovRel (&(MainState.motors[MOTOR_SYN]), step, speed);
								MainState.stateManageTop = STATE0_BASIC;
						}break;				
		case MSG_REGEHOST_MOTOR_INQUIRE:break;								
		case MSG_REGEHOST_MOTOR_MOVXY:{		
								MainState.stateManageTop = STATE0_MV;
								MainState.stateManageMid = STATE1_MV_BASIC;
								RegeStateMvMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_ZAXISSUB:{
								MainState.stateManageTop = STATE0_SUB;
								MainState.stateManageMid = STATE1_SUB_INIT;
								RegeStateSubMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_ZAXISDRA:{	
								MainState.stateManageTop = STATE0_DRA;
								MainState.stateManageMid = STATE1_DRA_BASIC ;
								RegeStateDraMessageHandler(msg);
						}break;
		case MSG_REGEHOST_MOTOR_ZAXISCLE:{	
								MainState.stateManageTop = STATE0_CLN;
								MainState.stateManageMid = STATE1_CLN_BASIC ;			
								RegeStateClnMessageHandler(msg);
						}break;
		default:break;	//something is wrong								
	}
}




