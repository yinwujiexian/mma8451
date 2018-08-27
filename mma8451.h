#ifndef __MMA8451_H
#define __MMA8451_H
#include "sys.h"

#define AD0_CTRL    PAout(15)

#define	STATUS_F_STATUS	0X00
#define	OUT_X_MSB	0X01
#define	OUT_X_LSB	0X02
#define	OUT_Y_MSB	0X03
#define	OUT_Y_LSB	0X04
#define	OUT_Z_MSB	0X05
#define	OUT_Z_LSB	0X06
#define	F_SETUP	0X09
#define	TRIG_CFG	0X0A
#define	SYSMOD	0X0B
#define	INT_SOURCE	0X0C
#define	WHO_AM_I	0X0D
#define	XYZ_DATA_CFG	0X0E
#define	HP_FILTER_CUTOFF	0X0F
#define	PL_STATUS 	0X10
#define	PL_CFG	0X11
#define	PL_COUNT	0X12
#define	PL_BF_ZCOMP	0X13
#define	P_L_THS_REG	0X14
#define	FF_MT_CFG	0X15
#define	FF_MT_SRC	0X16
#define	FF_MT_THS	0X17
#define	FF_MT_COUNT	0X18
#define	TRANSIENT_CFG	0X1D
#define	TRANSIENT_SRC	0X1E
#define	TRANSIENT_THS	0X1F
#define	TRANSIENT_COUNT	0X20
#define	PULSE_CFG	0X21
#define	PULSE_SRC	0X22
#define	PULSE_THSX	0X23
#define	PULSE_THSY	0X24
#define	PULSE_THSZ	0X25
#define	PULSE_TMLT	0X26
#define	PULSE_LTCY	0X27
#define	PULSE_WIND	0X28
#define	ASLP_COUNT	0X29
#define	CTRL_REG1	0X2A
#define	CTRL_REG2	0X2B
#define	CTRL_REG3	0X2C
#define	CTRL_REG4	0X2D
#define	CTRL_REG5	0X2E
#define	OFF_X	0X2F
#define	OFF_Y	0X30
#define	OFF_Z	0X31

//∂®“Âµÿ÷∑
#define MMA_ADDR 0X1C

u8 MMA_Init(void);
u8 MMA_Write_Byte(u8 reg,u8 data);
u8 MMA_Read_Byte(u8 reg);
u8 MMA_Write_Len(u8 addr, u8 reg, u8 len, u8 *buf);
u8 MMA_Read_Len(u8 addr, u8 reg, u8 len, u8 *buf);
u8 MMA_Get_Gyroscope(short *ax, short *ay, short *az);

#endif



