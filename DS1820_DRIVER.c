/***************************************************
   .c
   DS18B20器件通用文件，
   Created on: 2018年12月7日
   Author: Seal.Liu
   history:
     <author>   <time>    <version >    <desc>
       Seal     18/12/7      1.0          cread
 ****************************************************/
#include <driver/DS1820_DRIVER.h>

//保存温度转换的结果
float DS1820ReSult[4] = {0,0,0,0};

//CRC校验表
unsigned char CrcTable[256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53
};

/*******************************************
功能：18b20硬件IO初始化
传入参数： 无
返回参数： 无
********************************************/
unsigned char DS18B20hwinit(void)
{
    return DS18B20IOinit();

}

/*******************************************
功能：CRC计算
传入参数： *DataBuff 需要计算的数据起始地址
           Lenth     需要计算的数据长度 
返回参数： crc_data CRC计算结果
********************************************/
unsigned char CRCCalculate(unsigned char *DataBuff,unsigned char Lenth)
{
    unsigned char i,crc_data=0;
   for(i=0;i<Lenth;i++)
     crc_data = CrcTable[crc_data^DataBuff[i]];
    return (crc_data);
}

/*******************************************
功能：向18B20发送复位脉冲
传入参数： 无
返回参数： 无
********************************************/
void DS18B20_Rst(unsigned char SensorNum)
{
    DS18B20_Mode_Outnew(SensorNum);
    DS18B20_DQ_OUT_Low(SensorNum);
    Delay_us(500);
    DS18B20_DQ_OUT_High(SensorNum);
    Delay_us(15);
}

/*******************************************
功能：检测DS18B20复位应答
传入参数： None
返回参数： 1、应答超时
           0、应答正常
********************************************/
unsigned char DS18B20_Answer_Check(unsigned char SensorNum)
{
    unsigned char delay = 0;
    DS18B20_Mode_IPUnew(SensorNum); //主机设置为上拉输入
    Delay_us(10);
    while((DS18B20_DQ_IN(SensorNum))&&(delay < 100))
    {
        delay++;
        Delay_us(1);
    }
    //经过100us后,如果没有应答脉冲,退出函数
    if (delay >= 100)//Hu200
        return 0;
    else
        delay = 0;
    //有应答脉冲,且存在时间不超过240us
    while (!DS18B20_DQ_IN(SensorNum)&&delay < 240)
    {
        delay++;
        Delay_us(1);
    }
    if (delay >= 240)
        return 0;
    return 1;
}

/*******************************************
功能：返回当前数据IO的状态
传入参数： None
返回参数： 1、检测到高电平
           0、检测到低电平
********************************************/
unsigned char ReadPin18B20(unsigned char SensorNum)
{
    if(DS18B20_DQ_IN(SensorNum))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/*******************************************
功能：18B20位读取
传入参数： None
返回参数： 1、bit = 1
           0、bit = 0
********************************************/
unsigned char DS18B20_Read_Bit(unsigned char SensorNum)
{
    unsigned char data = 0;
    DS18B20_Mode_Outnew(SensorNum);
    DS18B20_DQ_OUT_Low(SensorNum); //读时间的起始:必须由主机产生 >1us <15us的低电平信号
    Delay_us(5);
    DS18B20_Mode_IPUnew(SensorNum); //设置成输入,释放总线,由外部上拉电阻将总线拉高
    Delay_us(10);
    if (ReadPin18B20(SensorNum))
        data = 1;
    else
        data = 0;
    Delay_us(60);
    DS18B20_DQ_OUT_High(SensorNum);
    return data;
}


/*******************************************
功能：18B20读取两个位
传入参数： None
返回参数： 读取结果
********************************************/
unsigned char DS18B20_Read_2Bit(unsigned char SensorNum)//读二位子程序
{
    unsigned char i;
    unsigned char dat = 0;
    for (i = 2; i > 0; i--)
    {
        dat = dat << 1;
        DS18B20_Mode_Outnew(SensorNum);

        DS18B20_DQ_OUT_Low(SensorNum); //读时间的起始:必须由主机产生 >1us <15us的低电平信号
        Delay_us(3);

        DS18B20_Mode_IPUnew(SensorNum);//设置成输入,释放总线,由外部上拉电阻将总线拉高

        Delay_us(10);

        if (ReadPin18B20(SensorNum))  dat |= 0x01;
        Delay_us(60);
        DS18B20_DQ_OUT_High(SensorNum);
    }
    return dat;
}

/*******************************************
功能：18B20读取一个字节
传入参数： None
返回参数： 读取结果
********************************************/
unsigned char DS18B20_Read_Byte(unsigned char SensorNum)  // read one byte
{
    unsigned char i, j, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        j = DS18B20_Read_Bit(SensorNum);
        dat = (dat) | (j << i);
    }
    return dat;
}


/*******************************************
功能：向18B20写一个位
传入参数： 写入位
返回参数： None
********************************************/
void DS18B20_Write_Bit(unsigned char SensorNum,unsigned char dat)
{
    DS18B20_Mode_Outnew(SensorNum);
    if (dat)
    {
        DS18B20_DQ_OUT_Low(SensorNum);// Write 1
        Delay_us(5);
        DS18B20_DQ_OUT_High(SensorNum);
        Delay_us(60);
    }
    else
    {
        DS18B20_DQ_OUT_Low(SensorNum);// Write 0
        Delay_us(60);
        DS18B20_DQ_OUT_High(SensorNum);
        Delay_us(2);
    }
}

/*******************************************
功能：向18B20写一个字节
传入参数： 写入字节
返回参数： None
********************************************/
void DS18B20_Write_Byte(unsigned char SensorNum,unsigned char dat)
{
    unsigned char j;
    unsigned char testb;
    DS18B20_Mode_Outnew(SensorNum);
    for (j = 1; j <= 8; j++)
    {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb)
        {
            DS18B20_DQ_OUT_Low(SensorNum);// Write 1
            Delay_us(7);
            DS18B20_DQ_OUT_High(SensorNum);
            Delay_us(50);
        }
        else
        {
            DS18B20_DQ_OUT_Low(SensorNum);// Write 0
            Delay_us(50);
            DS18B20_DQ_OUT_High(SensorNum);// 释放总线
            Delay_us(2);
        }
    }
}


/*******************************************
功能：18B20复位&应答检测
传入参数： 无
返回参数： 应答结果
********************************************/
unsigned char DS18B20_Init(unsigned char SensorNum)
{
    DS18B20_Rst(SensorNum);
    return DS18B20_Answer_Check(SensorNum);
}


/*******************************************
功能：启动18B20温度转换
传入参数： 无
返回参数： 0:启动失败
           1:启动成功
********************************************/
unsigned char DS18B20Start(unsigned char SensorNum)
{
    //OPen18B20_POWER();  
	//Delay_us(1000000);
	if(DS18B20_Init(SensorNum))
	{
		DS18B20_Write_Byte(SensorNum,0xcc);// skip rom
    	DS18B20_Write_Byte(SensorNum,0x44);// convert
		return 1;
	}
	return 0;
}

/*******************************************
功能：读取温度值
传入参数： float *ret_temperature  保存温度结果的地址
返回参数： 0:读取失败
           1:读取成功
********************************************/
unsigned char DS18B20_Get_Temp(unsigned char SensorNum, float *ret_temperature)
{

    unsigned char j;//匹配的字节
    unsigned char recv_temp[9] = {0},all_zero_check = 0;
    short Temperature = 0;
    float Temperature_flt= 0 ;

    DS18B20_Init(SensorNum);
    DS18B20_Write_Byte(SensorNum,0xcc);// skip rom
    DS18B20_Write_Byte(SensorNum,0xbe);//
    recv_temp[0] = DS18B20_Read_Byte(SensorNum); // LSB
    recv_temp[1] = DS18B20_Read_Byte(SensorNum); // MSB
    recv_temp[2] = DS18B20_Read_Byte(SensorNum);
    recv_temp[3] = DS18B20_Read_Byte(SensorNum);
    recv_temp[4] = DS18B20_Read_Byte(SensorNum);
    recv_temp[5] = DS18B20_Read_Byte(SensorNum);
    recv_temp[6] = DS18B20_Read_Byte(SensorNum);
    recv_temp[7] = DS18B20_Read_Byte(SensorNum);
    recv_temp[8] = DS18B20_Read_Byte(SensorNum);

    for(j=0;j<=8;j++)
    {
        all_zero_check |= recv_temp[j];
    }
	
    if(all_zero_check == 0)     //全零检测
        return 0; 
	
    if(CRCCalculate(recv_temp,8) != recv_temp[8]) //CRC校验
        return 0;
	
    if (recv_temp[1] & 0x80)
    {
        
        Temperature = (recv_temp[1] << 8) | recv_temp[0];
        Temperature_flt = (~Temperature) + 1;
        Temperature_flt *= 0.0625;
        Temperature_flt = -Temperature_flt;
    }
    else
    { 
        Temperature_flt = ((recv_temp[1] << 8) | recv_temp[0])*0.0625;
    }
    *ret_temperature = Temperature_flt;
	
    return 1;
}


/*******************************************
功能：读取温度Demo任务
传入参数： float *ret_temperature  保存温度结果的地址
返回参数：None
********************************************/


void Task_Demo_DS18B20( float *ret_temperature)
{
        OPen18B20_POWER();  //上电
        Task_sleep(500);    //等待电源稳定
        DS18B20Start(1);    //启动1号传感器
        DS18B20Start(2);    //启动2号传感器
        DS18B20Start(3);    //启动3号传感器
        DS18B20Start(4);    //启动4号传感器
        Task_sleep(1000);   //等待转换完成
        while(1)
        {
            DS18B20_Get_Temp(1,ret_temperature);   //获取1号传感器数据
            DS18B20_Get_Temp(4,ret_temperature+1); //获取2号传感器数据
            DS18B20_Get_Temp(2,ret_temperature+2); //获取3号传感器数据
            DS18B20_Get_Temp(3,ret_temperature+3); //获取4号传感器数据
            Task_sleep(10);
            DS18B20Start(1);  //启动下次转换
            DS18B20Start(2);
            DS18B20Start(3);
            DS18B20Start(4);
            Task_sleep(1000); //等待转换完成
        }

}





