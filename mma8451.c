#include "sys.h"
#include "delay.h"
#include "mma8451.h"
#include "usart.h"
#include "stm32f10x.h"
#include "mmaiic.h"

//初始化MMA8451
//返回值   0，正常    其余，错误代码
u8 MMA_Init(void)
{
    u8 res;
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //先使能外设IO PORTA时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	 // 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA

    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); //禁止JTAG,从而PA15可以做普通IO使用,否则PA15不能做普通IO!!!

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
//iic写一个字节
//reg：寄存器地址
//data：数据
//返回值：0，正常  其他，错误
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
//iic读一个字节
//reg：寄存器地址
//返回值：读到的数据
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
//iic连续写
//addr器件地址
//reg寄存器地址
//len写入数据长度
//buf数据区
//返回值 0，正常  其余，错误代码
u8 MMA_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    u8 i;
    MMA_IIC_Start();
    MMA_IIC_Send_Byte(addr << 1); //发送器件地址+写命令
    if(MMA_IIC_Wait_Ack())	//等待应答
    {
        MMA_IIC_Stop();
        return 1;
    }
    MMA_IIC_Send_Byte(reg);	//写寄存器地址
    MMA_IIC_Wait_Ack();		//等待应答
    for(i = 0; i < len; i++)
    {
        MMA_IIC_Send_Byte(buf[i]);	//发送数据
        if(MMA_IIC_Wait_Ack())		//等待ACK
        {
            MMA_IIC_Stop();
            return 1;
        }
    }
    MMA_IIC_Stop();
    return 0;
}
//iic连续读
//addr器件地址
//reg寄存器地址
//len要读取的数据长度
//buf读到的数据储存区
//返回值  0，正常   其余，错误代码
u8 MMA_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf)
{
    int i;
    MMA_IIC_Start();
    MMA_IIC_Send_Byte(addr << 1); //发送器件地址+写命令
    if(MMA_IIC_Wait_Ack())	//等待应答
    {
        MMA_IIC_Stop();
        return 1;
    }
    MMA_IIC_Send_Byte(reg);	//写寄存器地址
    MMA_IIC_Wait_Ack();		//等待应答
    MMA_IIC_Start();
    MMA_IIC_Send_Byte((addr << 1) + 1); //发送器件地址+读命令
    MMA_IIC_Wait_Ack();		//等待应答
    for(i = 0; i < len; i++)
    {   *buf  = MMA_IIC_Read_Byte();
        if(i == len - 1) MMA_IIC_NAck();
        else MMA_IIC_Ack();
		buf++;
    }
//    while(len)
//    {
//        if(len == 1)*buf = MMA_IIC_Read_Byte(0); //读数据,发送nACK
//        else *buf = MMA_IIC_Read_Byte(1);		//读数据,发送ACK
//        len--;
//        buf++;
//    }
    MMA_IIC_Stop();	//产生一个停止条件
    return 0;
}
//*********************************************************
//
//连续读出MMA8452内部加速度数据，地址范围0x01~0x06
//
//*********************************************************
//void Multiple_read_MMA8451(void)
//{   uchar i;
//    MMA8452_Start();                          //起始信号
//    MMA8452_SendByte(SlaveAddress);           //发送设备地址+写信号
//    MMA8452_SendByte(0x01);                   //发送存储单元地址，从0x01开始
//    MMA8452_Start();                          //起始信号
//    MMA8452_SendByte(SlaveAddress+1);         //发送设备地址+读信号
//	 for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
//    {
//        BUF[i] = MMA8452_RecvByte();          //BUF[0]存储0x32地址中的数据
//        if (i == 5)
//        {
//           MMA8452_SendACK(1);                //最后一个数据需要回NOACK
//        }
//        else
//        {
//          MMA8452_SendACK(0);                //回应ACK
//       }
//   }
//    MMA8452_Stop();                          //停止信号
//    Delay5ms();
//}



//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
u8 MMA_Get_Gyroscope(short *gx, short *gy, short *gz)
{
    u8 buf[6];
    MMA_Read_Len(MMA_ADDR, OUT_X_MSB , 6, buf);
    *gx = ((short)((buf[0] << 8) | buf[1])) ;
    *gy = ((short)((buf[2] << 8) | buf[3])) ;
    *gz = ((short)((buf[4] << 8) | buf[5])) ;
    return 0;
}

