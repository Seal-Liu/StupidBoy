#ifndef USER18B20_H
#define USER18B20_H

/**************************************************/
/*********************ͷ�ļ�***********************/
/**************************************************/

#include <ti/drivers/PIN.h>
#include <ti/devices/cc13x0/driverlib/ioc.h>
#include <ti/drivers/Power.h>
#include <ti/drivers/power/PowerCC26XX.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/sysbios/knl/Task.h>


/**************************************************/
/*********************IO����***********************/
/**************************************************/

#define  POWERIOId  26        //��ԴIO
#define  DataIdID(x) x+18     //������ʼIO


#define  DS18B20_DQ_OUT_Low(x)       PIN_setOutputValue(DS18B20PinHandle,PIN_ID(DataIdID(x)) ,0)  //����IO����͵�ƽ
#define  DS18B20_DQ_OUT_High(x)      PIN_setOutputValue(DS18B20PinHandle,PIN_ID(DataIdID(x)) ,1)  //����IO����ߵ�ƽ
#define  DS18B20_DQ_IN(x)            PIN_getInputValue(PIN_ID(DataIdID(x))) //��ȡ����IO��ƽ



/**************************************************/
/*******************Ӳ�������*********************/
/**************************************************/
 extern PIN_Handle DS18B20PinHandle;  //IOHandle
 extern PIN_State  DS18B20PinState;   //IO״̬


 extern void DS18B20_Mode_IPUnew(unsigned char i);               //����IO�л�������ģʽ
 extern void DS18B20_Mode_Outnew(unsigned char i);
 extern PIN_Status OPen18B20_POWER(void);                       //����DS18B20��Դ
 extern PIN_Status CLose18B20_POWER(void);                      //�ر�DS18B20��Դ
 extern void Delay_us(unsigned int N_us);                       //uS��ʱ
 extern unsigned char DS18B20IOinit(void);                      //IO�ڳ�ʼ��

/**************************************************/
/******************Ӧ�ò�ӿ�**********************/
/**************************************************/
extern float DS1820ReSult[4];
extern PIN_Status OPen18B20_POWER(void);					   //����DS18B20��Դ
extern PIN_Status CLose18B20_POWER(void);					   //�ر�DS18B20��Դ
extern unsigned char DS18B20hwinit(void);                      //Ӳ����ʼ��
extern unsigned char DS18B20Start(unsigned char SensorNum);      //����ת��
extern unsigned char DS18B20_Get_Temp(unsigned char SensorNum, float *ret_temperature);  //��ȡ�¶�
extern void Task_Demo_DS18B20( float *ret_temperature);

#endif



