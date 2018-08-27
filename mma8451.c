#include "sys.h"
#include "delay.h"
#include "mma8451.h"
#include "usart.h"
#include "stm32f10x.h"
#include "mmaiic.h"

//��ʼ��MMA8451
//����ֵ   0������    ���࣬�������
u8 MMA_Init(void)
{
    u8 res;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //ʹ��AFIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //��ʹ������IO PORTAʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 // �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //��ֹJTAG,�Ӷ�PA15��������ͨIOʹ��,����PA15��������ͨIO!!!

    AD0_CTRL = 0;

    MMA_IIC_Init();
    MMA_Write_Byte(CTRL_REG1, 0x01);
    delay_ms(100);
    MMA_Write_Byte(CTRL_REG2, 0x02);
    delay_ms(100);
    MMA_Write_Byte(XYZ_DATA_CFG, 0x10);
    res = MMA_Read_Byte(WHO_AM_I);
    if(res == MMA_ADDR)
    {
        MMA_Write_Byte(CTRL_REG1, 0x01);
        MMA_Write_Byte(CTRL_REG2, 0x02);
        MMA_Write_Byte(XYZ_DATA_CFG, 0x10);
    } else return 1;
    return 0;
}
//iicдһ���ֽ�
//reg���Ĵ�����ַ
//data������
//����ֵ��0������  ����������
u8 MMA_Write_Byte(u8 reg, u8 data)
{
    MMA_IIC_Start();
    MMA_IIC_Send_Byte(MMA_ADDR << 1);
    if (MMA_IIC_Wait_Ack())
    {
        MMA_IIC_Stop();
        return 1;
    }
    MMA_IIC_Send_Byte(reg);
    MMA_IIC_Wait_Ack();
    MMA_IIC_Send_Byte(data);
    if (MMA_IIC_Wait_Ack())
    {
        MMA_IIC_Stop();
        return 1;
    }
    MMA_IIC_Stop();
    return 0;
}
//iic��һ���ֽ�
//reg���Ĵ�����ַ
//����ֵ������������
u8 MMA_Read_Byte(u8 reg)
{
    MMA_IIC_Start();
    MMA_IIC_Send_Byte(MMA_ADDR << 1 );
    MMA_IIC_Wait_Ack();
    MMA_IIC_Send_Byte(reg);
    MMA_IIC_Wait_Ack();
    MMA_IIC_Start();
    MMA_IIC_Send_Byte((MMA_ADDR << 1)|1);
    MMA_IIC_Wait_Ack();
    MMA_IIC_Read_Byte();
    MMA_IIC_NAck();
    MMA_IIC_Stop();
    return 0;
}
//iic����д
//addr������ַ
//reg�Ĵ�����ַ
//lenд�����ݳ���
//buf������
//����ֵ 0������  ���࣬�������
u8 MMA_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i;
    MMA_IIC_Start();
    MMA_IIC_Send_Byte(addr << 1); //����������ַ+д����
    if(MMA_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
        MMA_IIC_Stop();
        return 1;
    }
    MMA_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MMA_IIC_Wait_Ack();		//�ȴ�Ӧ��
    for(i = 0; i < len; i++)
    {
        MMA_IIC_Send_Byte(buf[i]);	//��������
        if(MMA_IIC_Wait_Ack())		//�ȴ�ACK
        {
            MMA_IIC_Stop();
            return 1;
        }
    }
    MMA_IIC_Stop();
    return 0;
}
//iic������
//addr������ַ
//reg�Ĵ�����ַ
//lenҪ��ȡ�����ݳ���
//buf���������ݴ�����
//����ֵ  0������   ���࣬�������
u8 MMA_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    int i;
    MMA_IIC_Start();
    MMA_IIC_Send_Byte(addr << 1); //����������ַ+д����
    if(MMA_IIC_Wait_Ack())	//�ȴ�Ӧ��
    {
        MMA_IIC_Stop();
        return 1;
    }
    MMA_IIC_Send_Byte(reg);	//д�Ĵ�����ַ
    MMA_IIC_Wait_Ack();		//�ȴ�Ӧ��
    MMA_IIC_Start();
    MMA_IIC_Send_Byte((addr << 1) + 1); //����������ַ+������
    MMA_IIC_Wait_Ack();		//�ȴ�Ӧ��
    for(i = 0; i < len; i++)
    {   *buf  = MMA_IIC_Read_Byte();
        if(i == len - 1) MMA_IIC_NAck();
        else MMA_IIC_Ack();
		buf++;
    }
//    while(len)
//    {
//        if(len == 1)*buf = MMA_IIC_Read_Byte(0); //������,����nACK
//        else *buf = MMA_IIC_Read_Byte(1);		//������,����ACK
//        len--;
//        buf++;
//    }
    MMA_IIC_Stop();	//����һ��ֹͣ����
    return 0;
}
//*********************************************************
//
//��������MMA8452�ڲ����ٶ����ݣ���ַ��Χ0x01~0x06
//
//*********************************************************
//void Multiple_read_MMA8451(void)
//{   uchar i;
//    MMA8452_Start();                          //��ʼ�ź�
//    MMA8452_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
//    MMA8452_SendByte(0x01);                   //���ʹ洢��Ԫ��ַ����0x01��ʼ
//    MMA8452_Start();                          //��ʼ�ź�
//    MMA8452_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
//	 for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
//    {
//        BUF[i] = MMA8452_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
//        if (i == 5)
//        {
//           MMA8452_SendACK(1);                //���һ��������Ҫ��NOACK
//        }
//        else
//        {
//          MMA8452_SendACK(0);                //��ӦACK
//       }
//   }
//    MMA8452_Stop();                          //ֹͣ�ź�
//    Delay5ms();
//}



//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ:0,�ɹ�
//    ����,�������
u8 MMA_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    u8 buf[6];
    MMA_Read_Len(MMA_ADDR, OUT_X_MSB , 6, buf);
    *gx = ((short)((buf[0] << 8) | buf[1])) ;
    *gy = ((short)((buf[2] << 8) | buf[3])) ;
    *gz = ((short)((buf[4] << 8) | buf[5])) ;
    return 0;
}

