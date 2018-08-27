#ifndef __MMAIIC_H
#define __MMAIIC_H
#include "sys.h"

#define MMA_SDA_IN()   {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define MMA_SDA_OUT()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

#define MMA_IIC_SCL  PBout(10)
#define MMA_IIC_SDA  PBout(11)
#define MMA_READ_SDA PBin(11)

void MMA_IIC_Init(void);
void MMA_IIC_Start(void);
void MMA_IIC_Stop(void);
u8 MMA_IIC_Wait_Ack(void);
void MMA_IIC_Ack(void);
void MMA_IIC_NAck(void);
void MMA_IIC_Send_Byte(u8 txd);
u8 MMA_IIC_Read_Byte(void);
#endif

