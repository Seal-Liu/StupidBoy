/***************************************************
   .c
   DS18B20����ͨ���ļ���
   Created on: 2018��12��7��
   Author: Seal.Liu
   history:
     <author>   <time>    <version >    <desc>
       Seal     18/12/7      1.0          cread
 ****************************************************/
#include <driver/DS1820_DRIVER.h>

//�����¶�ת���Ľ��
float DS1820ReSult[4] = {0,0,0,0};

//CRCУ���
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
���ܣ�18b20Ӳ��IO��ʼ��
��������� ��
���ز����� ��
********************************************/
unsigned char DS18B20hwinit(void)
{
    return DS18B20IOinit();

}

/*******************************************
���ܣ�CRC����
��������� *DataBuff ��Ҫ�����������ʼ��ַ
           Lenth     ��Ҫ��������ݳ��� 
���ز����� crc_data CRC������
********************************************/
unsigned char CRCCalculate(unsigned char *DataBuff,unsigned char Lenth)
{
    unsigned char i,crc_data=0;
   for(i=0;i<Lenth;i++)
     crc_data = CrcTable[crc_data^DataBuff[i]];
    return (crc_data);
}

/*******************************************
���ܣ���18B20���͸�λ����
��������� ��
���ز����� ��
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
���ܣ����DS18B20��λӦ��
��������� None
���ز����� 1��Ӧ��ʱ
           0��Ӧ������
********************************************/
unsigned char DS18B20_Answer_Check(unsigned char SensorNum)
{
    unsigned char delay = 0;
    DS18B20_Mode_IPUnew(SensorNum); //��������Ϊ��������
    Delay_us(10);
    while((DS18B20_DQ_IN(SensorNum))&&(delay < 100))
    {
        delay++;
        Delay_us(1);
    }
    //����100us��,���û��Ӧ������,�˳�����
    if (delay >= 100)//Hu200
        return 0;
    else
        delay = 0;
    //��Ӧ������,�Ҵ���ʱ�䲻����240us
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
���ܣ����ص�ǰ����IO��״̬
��������� None
���ز����� 1����⵽�ߵ�ƽ
           0����⵽�͵�ƽ
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
���ܣ�18B20λ��ȡ
��������� None
���ز����� 1��bit = 1
           0��bit = 0
********************************************/
unsigned char DS18B20_Read_Bit(unsigned char SensorNum)
{
    unsigned char data = 0;
    DS18B20_Mode_Outnew(SensorNum);
    DS18B20_DQ_OUT_Low(SensorNum); //��ʱ�����ʼ:�������������� >1us <15us�ĵ͵�ƽ�ź�
    Delay_us(5);
    DS18B20_Mode_IPUnew(SensorNum); //���ó�����,�ͷ�����,���ⲿ�������轫��������
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
���ܣ�18B20��ȡ����λ
��������� None
���ز����� ��ȡ���
********************************************/
unsigned char DS18B20_Read_2Bit(unsigned char SensorNum)//����λ�ӳ���
{
    unsigned char i;
    unsigned char dat = 0;
    for (i = 2; i > 0; i--)
    {
        dat = dat << 1;
        DS18B20_Mode_Outnew(SensorNum);

        DS18B20_DQ_OUT_Low(SensorNum); //��ʱ�����ʼ:�������������� >1us <15us�ĵ͵�ƽ�ź�
        Delay_us(3);

        DS18B20_Mode_IPUnew(SensorNum);//���ó�����,�ͷ�����,���ⲿ�������轫��������

        Delay_us(10);

        if (ReadPin18B20(SensorNum))  dat |= 0x01;
        Delay_us(60);
        DS18B20_DQ_OUT_High(SensorNum);
    }
    return dat;
}

/*******************************************
���ܣ�18B20��ȡһ���ֽ�
��������� None
���ز����� ��ȡ���
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
���ܣ���18B20дһ��λ
��������� д��λ
���ز����� None
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
���ܣ���18B20дһ���ֽ�
��������� д���ֽ�
���ز����� None
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
            DS18B20_DQ_OUT_High(SensorNum);// �ͷ�����
            Delay_us(2);
        }
    }
}


/*******************************************
���ܣ�18B20��λ&Ӧ����
��������� ��
���ز����� Ӧ����
********************************************/
unsigned char DS18B20_Init(unsigned char SensorNum)
{
    DS18B20_Rst(SensorNum);
    return DS18B20_Answer_Check(SensorNum);
}


/*******************************************
���ܣ�����18B20�¶�ת��
��������� ��
���ز����� 0:����ʧ��
           1:�����ɹ�
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
���ܣ���ȡ�¶�ֵ
��������� float *ret_temperature  �����¶Ƚ���ĵ�ַ
���ز����� 0:��ȡʧ��
           1:��ȡ�ɹ�
********************************************/
unsigned char DS18B20_Get_Temp(unsigned char SensorNum, float *ret_temperature)
{

    unsigned char j;//ƥ����ֽ�
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
	
    if(all_zero_check == 0)     //ȫ����
        return 0; 
	
    if(CRCCalculate(recv_temp,8) != recv_temp[8]) //CRCУ��
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
���ܣ���ȡ�¶�Demo����
��������� float *ret_temperature  �����¶Ƚ���ĵ�ַ
���ز�����None
********************************************/


void Task_Demo_DS18B20( float *ret_temperature)
{
        OPen18B20_POWER();  //�ϵ�
        Task_sleep(500);    //�ȴ���Դ�ȶ�
        DS18B20Start(1);    //����1�Ŵ�����
        DS18B20Start(2);    //����2�Ŵ�����
        DS18B20Start(3);    //����3�Ŵ�����
        DS18B20Start(4);    //����4�Ŵ�����
        Task_sleep(1000);   //�ȴ�ת�����
        while(1)
        {
            DS18B20_Get_Temp(1,ret_temperature);   //��ȡ1�Ŵ���������
            DS18B20_Get_Temp(4,ret_temperature+1); //��ȡ2�Ŵ���������
            DS18B20_Get_Temp(2,ret_temperature+2); //��ȡ3�Ŵ���������
            DS18B20_Get_Temp(3,ret_temperature+3); //��ȡ4�Ŵ���������
            Task_sleep(10);
            DS18B20Start(1);  //�����´�ת��
            DS18B20Start(2);
            DS18B20Start(3);
            DS18B20Start(4);
            Task_sleep(1000); //�ȴ�ת�����
        }

}





