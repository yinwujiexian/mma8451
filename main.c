#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "mma8451.h"
#include "mmaiic.h"
#include "usart.h"
#include "lcd.h"
//int filter(int ch)
//{
//    short ax, ay, az;
//    char i, j, k;
//    char temp, sum = 0, value[3];
//    int value_buf[12][3];
//    for(i = 0; i < 12; i++)
//    {
//        MMA_Get_Gyroscope(&ax, &ay, &az);
//        value_buf[i][0] = ax;
//        value_buf[i][1] = ay;
//        value_buf[i][2] = az;
//        delay_ms(1);
//    }
//    for(k = 0; k < 3; k++)
//    {
//        for(j = 0; j < 11; j++)
//        {
//            for(i = 0; i < 12 - j; i++)
//            {
//                if(value_buf[i][k] > value_buf[i + 1][k])
//                {
//                    temp = value_buf[i][k];
//                    value_buf[i][k] = value_buf[i + 1][k];
//                    value_buf[i + 1] [k] = temp;
//                }
//            }
//        }
//    }
//    for(k = 0; k < 11; k++)
//    {
//        for(i = 1; i < 11; i++)
//            {sum += value_buf[i][k];}
//        value[k] = sum / (12 - 2);
//    }
//    return(value[ch]);
//}
int main (void)
{
	short ax, ay, az;
//    int result[3], i;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);
    delay_init();
    LCD_Init();
    MMA_Init();
    POINT_COLOR = RED;
    while(1)
    {
//        for(i = 0; i < 3; i++)
//        {
//            result[i] = filter(i);
//        }
		MMA_Get_Gyroscope(&ax, &ay, &az);
        printf("A=%d\r\n", ax );
        if(ax < 0)
        {
            ax = (-1) * ax;
            LCD_ShowString(20, 100, 20, 20, 16, "-");
            LCD_ShowxNum(30, 100, ax, 5, 16, 0);
        }
        else
        {
            LCD_ShowString(20, 100, 20, 20, 16, "+");
            LCD_ShowxNum(30, 100, ax, 5, 16, 0);
        }
        printf("B=%d\r\n", ay);
        if(ay < 0)
        {
            ay = -ay;
            LCD_ShowString(20, 120, 20, 20, 16, "-");
            LCD_ShowxNum(30, 120, ay, 5, 16, 0);
        }
        else
        {
            LCD_ShowString(20, 120, 20, 20, 16, "+");
            LCD_ShowxNum(30, 120, ay, 5, 16, 0);
        }
        printf("C=%d\r\n", az);
        if(az < 0)
        {
            az = -az;
            LCD_ShowString(20, 140, 20, 20, 16, "-");
            LCD_ShowxNum(30, 140, az, 5, 16, 0);
        } else
        {
            LCD_ShowString(20, 140, 20, 20, 16, "+");
            LCD_ShowxNum(30, 140, az, 5, 16, 0);
        }
    }
}
