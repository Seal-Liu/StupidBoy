#ifndef USER18B20_H
#define USER18B20_H

/**************************************************/
/*********************头文件***********************/
/**************************************************/

#include <ti/drivers/PIN.h>
#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/sysbios/knl/Task.h>


/**************************************************/
/*********************IO定义***********************/
/**************************************************/

#define  POWERIOId  26        //电源IO
#define  DataIdID(x) x+18     //数据起始IO


#define  DS18B20_DQ_OUT_Low(x)       PIN_setOutputValue(DS18B20PinHandle,PIN_ID(DataIdID(x)) ,0)  //数据IO输出低电平
#define  DS18B20_DQ_OUT_High(x)      PIN_setOutputValue(DS18B20PinHandle,PIN_ID(DataIdID(x)) ,1)  //数据IO输出高电平
#define  DS18B20_DQ_IN(x)            PIN_getInputValue(PIN_ID(DataIdID(x))) //获取数据IO电平



/**************************************************/
/*******************硬件抽象层*********************/
/**************************************************/
 extern PIN_Handle DS18B20PinHandle;  //IOHandle
 extern PIN_State  DS18B20PinState;   //IO状态


 extern void DS18B20_Mode_IPUnew(unsigned char i);               //数据IO切换至输入模式
 extern void DS18B20_Mode_Outnew(unsigned char i);
 extern PIN_Status OPen18B20_POWER(void);                       //启动DS18B20电源
 extern PIN_Status CLose18B20_POWER(void);                      //关闭DS18B20电源
 extern void Delay_us(unsigned int N_us);                       //uS延时
 extern unsigned char DS18B20IOinit(void);                      //IO口初始化

/**************************************************/
/******************应用层接口**********************/
/**************************************************/
extern float DS1820ReSult[4];
extern PIN_Status OPen18B20_POWER(void);					   //启动DS18B20电源
extern PIN_Status CLose18B20_POWER(void);					   //关闭DS18B20电源
extern unsigned char DS18B20hwinit(void);                      //硬件初始化
extern unsigned char DS18B20Start(unsigned char SensorNum);      //启动转换
extern unsigned char DS18B20_Get_Temp(unsigned char SensorNum, float *ret_temperature);  //获取温度
extern void Task_Demo_DS18B20( float *ret_temperature);

#endif



