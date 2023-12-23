/* File:   I2C_Support.h */
#ifndef I2C_SUPPORT_H
#define	I2C_SUPPORT_H
void DS1621_Init(); //
void DS3231_Read_Time(); //
void DS3231_Write_Time();
void DS3231_Read_Alarm_Time();
void DS3231_Write_Alarm_Time();
int DS3231_Read_Temp();
int DS1621_Read_Temp(); //
void DS3231_Turn_On_Alarm();
void DS3231_Turn_Off_Alarm();
void DS3231_Init();
void DS3231_Write_Initial_Alarm_Time();
#endif	/* I2C_SUPPORT_H */