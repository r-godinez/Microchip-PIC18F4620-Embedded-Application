/* File:   main.h */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>
#include "I2C.h"
#include "I2C_Support.h"
#include "Interrupt.h"
#include "Fan_Support.h"
#include "ST7735_TFT.h"
#include "utils.h"
#include "Main_Screen.h"
#include "Setup_Alarm_Time.h"
#include "Setup_Fan_Temp.h"
#include "Setup_Time.h"

#ifndef MAIN_H
#define _XTAL_FREQ      8000000
#define ACK             1
#define NAK             0
#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02
//#define SEC_LED       PORTCbits.RC4
//#define FANEN_LED     PORTAbits.RA4
#define RTC_ALARM_NOT   PORTDbits.RD5
#define FAN_EN			PORTAbits.RA5   // configure portA bit 5 for FAN_EN
#define FAN_EN_LED		PORTAbits.RA4   // configure portA bit 4 for FAN_EN_LED
#define FAN_PWM			PORTCbits.RC2   // configure portC bit 2 for FAN PWM
#define FAN_EN          PORTAbits.RA5
#define KEY_PRESSED_LED PORTAbits.RA1

#define Setup_Time_Key        	12
#define Setup_Alarm_Key       	13
#define Setup_Fan_Temp_Key    	14
#define Toggle_Fan_Key      	5

#define Ch_Minus                0
#define Channel                 1
#define Ch_Plus                 2
#define Prev                    3
#define Next                    4
#define Play_Pause              5
#define Minus                   6
#define Plus                    7
#define EQ                      8
#endif	/* MAIN_H */