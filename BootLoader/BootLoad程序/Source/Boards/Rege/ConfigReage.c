/*********************************************************
 * Shenzhen Thistroy Bio. Ltd. Co. copyright
 * -- Created by software, DO NOT Modify
 ********************************************************/

#include "Config.h"
#include "ConfigReage.h"

CONST ConfigDefault configDefault[CFG_MAX-CFG_USER_BEGIN] = {
	{       -3952,       -32500,        32500},// 垂直负向极限位置
	{         384,       -32500,        32500},// 垂直正向极限位置
	{           0,       -32000,        32000},// 垂直初始位置
	{       -3520,       -32000,        32000},// 清洗池上方垂直位置
	{           0,       -32000,        32000},// 清洗池清洗垂直位置
	{           0,       -32000,        10000},// 清洗池垂直极限位置
	{       -1728,       -32000,        32000},// 冷藏试剂大孔区上方垂直位置
	{           0,       -32000,        32000},// 冷藏试剂大孔区垂直极限位置
	{       -1728,       -32000,        32000},// 冷藏试剂中孔区上方垂直位置
	{           0,       -32000,        32000},// 冷藏试剂中孔区垂直极限位置
	{       -1728,       -32000,        32000},// 冷藏试剂小孔区上方垂直位置
	{           0,       -32000,        32000},// 冷藏试剂小孔区垂直极限位置
	{       -2080,       -32000,        32000},// 配合抓杯手上方垂直位置
	{           0,       -32000,        32000},// 配合抓杯手垂直位置
	{           0,       -32000,        32000},// 配合抓杯手垂直极限位置
	{      -32000,       -32500,            0},// X轴负向极限位置
	{       32000,            0,        32000},// X轴正向极限位置
	{         200,       -32000,        32000},// X轴初始位置
	{      -32000,       -32500,            0},// Y轴负向极限位置
	{       32000,            0,        32000},// Y轴正向极限位置
	{         200,       -32000,        32000},// Y轴初始位置
	{         732,       -32000,        32000},// 清洗池位置-X轴
	{       -6247,       -32000,        32000},// 冷藏试剂区域调试位置1-X轴
	{       -6039,       -32000,        32000},// 冷藏试剂区域调试位置2-X轴
	{        6604,       -32000,        32000},// 冷藏试剂区域调试位置3-X轴
	{       -6004,       -32000,        32000},// 配合抓杯手-X轴
	{       -1652,       -32000,        32000},// 清洗池位置-Y轴
	{        2085,       -32000,        32000},// 冷藏试剂区域调试位置1-Y轴
	{       12437,       -32000,        32000},// 冷藏试剂区域调试位置2-Y轴
	{       10048,       -32000,        32000},// 冷藏试剂区域调试位置3-Y轴
	{       -1924,       -32000,        32000},// 配合抓杯手-Y轴
	{          20,            0,          100},// 内壁清洗时间
	{          20,            0,          100},// 外壁清洗时间
	{          10,            0,          100},// 吸液时间
	{          10,            0,          100},// 排液时间
	{          10,            0,          100},// 排试剂下降时间
	{           5,            0,          100},// 吸试剂下降时间
	{          10,            0,          100},// 吸液前吸空气量
	{           0,            0,          100},// 多吸液量
	{           0,            0,          100},// 吸液后回推量
	{          10,            0,         3200},// 吸液后吸空气量
	{          10,            0,         3200},// 排液后吸空气量
	{          50,            0,         3200},// 清洗后吸空气量
	{        3200,       -32000,        32000},// 注射器负向极限位置
	{        2560,        -3200,         3200},// 注射器初始位置
	{         100,            0,        32000},// 注射器正向极限位置
	{         128,            0,          516},// 1微升对应微步数
	{           0,            0,            0},// 堵针检测阈值1
	{           0,            0,            0},// 堵针检测阈值2
	{           0,            0,            0},// 堵针检测阈值3
	{           0,            0,            0},// 堵针检测阈值4
	{           0,            0,            0},// 堵针检测阈值5
	{         128,            0,         1000},// 检测到液面后Z轴下降步数
	{           0,            0,            0},// 试剂预热理论目标温度
	{           0,            0,            0},// 是否开启环境温度动态补偿
	{           0,            0,            0},// 传感器温度与试剂温度偏差
	{           0,            0,            0},// 传感器与试剂温度偏差-室温15度
	{           0,            0,            0},// 传感器与试剂温度偏差-室温30度
	{           0,            0,            0},// PID的比例系数 
	{           0,            0,            0},// PID的积分时间
	{           0,            0,            0},// PID的微分时间
	{           0,            0,            0},// 是否开启清洗预热补偿
	{           0,            0,            0},// 是否开启吸试剂预热补偿
	{           0,            0,            0},// 传感器R0
	{           0,            0,            0},// 传感器R37
	{           0,            0,            0},// 传感器R100
	{           0,            0,            0},// 传感器调整参数1-保留
	{           0,            0,            0},// 传感器调整参数2-保留
	{           0,            0,            0},// 传感器短路温度
	{           0,            0,            0},// 传感器断路温度
	{        1750,            0,        32000},// 总吸液量
};
