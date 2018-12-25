/***************************************************
   DS18B20cfg.c
   配置18B20的硬件，
   包括电源控制，数据IO，以及延时参数
   本文件配置适合CC1350,固件库版本 1.60.00.21
   Created on: 2018年12月7日
   Author: Seal.Liu
   history:
     <author>   <time>    <version >    <desc>
       Seal     18/12/7      1.0          cread
 ****************************************************/
 #include <driver/DS1820_DRIVER.h>
#include "config/config.h"

PIN_Handle DS18B20PinHandle;
PIN_State  DS18B20PinState;

//  18B20IO配置表
 PIN_Config DS18B20_IoInitTable[] =
 {
       PIN_ID(POWERIOId)           | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
       PIN_ID(DataIdID(1))         | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,
       PIN_ID(DataIdID(2))         | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,
       PIN_ID(DataIdID(3))         | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,
       PIN_ID(DataIdID(4))         | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_DIS,
       PIN_TERMINATE
 };

 //数据IO切换至输入模式
 void DS18B20_Mode_IPUnew(unsigned char i)
 {
     PIN_setConfig(DS18B20PinHandle, PIN_BM_OUTPUT_MODE | PIN_BM_INPUT_MODE,PIN_ID(DataIdID(i))|PIN_GPIO_OUTPUT_DIS | PIN_INPUT_EN | PIN_PULLUP);
 }

//数据IO切换至输出模式
void DS18B20_Mode_Outnew(unsigned char i)
{
    PIN_setConfig(DS18B20PinHandle,PIN_BM_OUTPUT_MODE | PIN_BM_INPUT_MODE,PIN_ID(DataIdID(i))|PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL|PIN_DRVSTR_MED);
}

//开启18B20电源
PIN_Status OPen18B20_POWER(void)
{
   return PIN_setOutputValue(DS18B20PinHandle,PIN_ID(POWERIOId) ,1);
}
//关闭18B20电源
PIN_Status CLose18B20_POWER(void)
{
   return PIN_setOutputValue(DS18B20PinHandle,PIN_ID(POWERIOId) ,0);
}

//1us延时
void Delay_us(unsigned int N_us)
{
    CPUdelay(12*N_us);
}

//IO初始化，获取IO句柄
unsigned char DS18B20IOinit(void)
{
	DS18B20PinHandle = PIN_open(&DS18B20PinState, DS18B20_IoInitTable);
	if(DS18B20PinHandle == 0)
		return 0;
	return 1;
}





