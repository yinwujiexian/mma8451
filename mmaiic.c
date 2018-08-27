#include "mmaiic.h"
#include "delay.h"
#include "stm32f10x.h"

void MMA_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStr;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStr.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStr.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStr.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStr);
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}
//����IIC��ʼ�ź�
void MMA_IIC_Start(void)
{
    MMA_SDA_OUT();
    MMA_IIC_SDA = 1;
    MMA_IIC_SCL = 1;
    delay_ms(1);
    MMA_IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
    delay_ms(1);
    MMA_IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void MMA_IIC_Stop(void)
{
    MMA_SDA_OUT();//sda�����
    MMA_IIC_SCL = 0;
    MMA_IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    delay_ms(1);
    MMA_IIC_SCL = 1;
    MMA_IIC_SDA = 1; //����I2C���߽����ź�
    delay_ms(1);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MMA_IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    MMA_SDA_IN();      //SDA����Ϊ����
    MMA_IIC_SDA = 1;
    delay_ms(1);
    MMA_IIC_SCL = 1;
    delay_ms(1);
    while(MMA_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
            MMA_IIC_Stop();
            return 1;
        }
    }
    MMA_IIC_SCL = 0; //ʱ�����0
    return 0;
}
//����ACKӦ��
void MMA_IIC_Ack(void)
{
    MMA_IIC_SCL = 0;
    MMA_SDA_OUT();
    MMA_IIC_SDA = 0;
    delay_ms(1);
    MMA_IIC_SCL = 1;
    delay_ms(1);
    MMA_IIC_SCL = 0;
}
//������ACKӦ��
void MMA_IIC_NAck(void)
{
    MMA_IIC_SCL = 0;
    MMA_SDA_OUT();
    MMA_IIC_SDA = 1;
    delay_ms(1);
    MMA_IIC_SCL = 1;
    delay_ms(1);
    MMA_IIC_SCL = 0;
}
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void MMA_IIC_Send_Byte(u8 txd)
{
    u8 t;
    MMA_SDA_OUT();
    for(t = 0; t < 8; t++)
    {
		delay_ms(1);
		if(txd&0x80) MMA_IIC_SDA=1;
		else MMA_IIC_SDA=0;
		delay_ms(1);
		MMA_IIC_SCL=1;
		delay_ms(1);
		MMA_IIC_SCL=0;
		txd<<=1;
//        MMA_IIC_SDA = (txd & 0x80)>>7;
//        txd <<= 1;
//        MMA_IIC_SCL = 1;
//        delay_ms(1);
//        MMA_IIC_SCL = 0;
//        delay_ms(1);
    }
}
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 MMA_IIC_Read_Byte(void)
{
    unsigned char i, data = 0;
	char  temp;
    MMA_SDA_IN();//SDA����Ϊ����
    for(i = 0; i < 8; i++ )
    {
		data<<=1;
		delay_ms(1);
		MMA_IIC_SCL=1;
		temp=MMA_READ_SDA;
		if(temp)   data++;
        delay_ms(1);   
        MMA_IIC_SCL = 0;            
    }
    return data;
		
//        MMA_IIC_SCL = 1;
//        delay_ms(1);
//        MMA_IIC_SCL = 0;
//        receive <<= 1;
//        if(MMA_READ_SDA)receive++;
//        delay_ms(1);
//    }
//    if (!ack)
//        MMA_IIC_NAck();//����nACK
//    else
//        MMA_IIC_Ack(); //����ACK
//    return receive;
}

