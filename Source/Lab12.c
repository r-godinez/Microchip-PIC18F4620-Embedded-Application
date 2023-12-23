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
#include "Main.h"
#include "ST7735_TFT.h"
#include "utils.h"
#pragma config OSC = INTIO67
#pragma config BOREN =OFF
#pragma config WDT=OFF
#pragma config LVP=OFF

#define KEY_PRESS_LED PORTEbits.RE0
#define KEY_PRESS PORTAbits.RA3
#define RD           1
#define GR           2
#define YL  		 3
#define BL           4
#define MA           5
#define CY			 6
#define WH           7
#define Minus_Key         6
#define Plus_Key          7
#define Play_Pause_Key    5
#define EQ_Key            8

void test_alarm();
char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;

char found;
char tempSecond = 0xff;
signed int DS1621_tempC, DS1621_tempF;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
unsigned char fan_set_temp = 75;

float volt;
char INT0_flag, INT1_flag, INT2_flag;

short Nec_ok = 0;
char Nec_button;
char FAN;
char duty_cycle;
int rps;
int rpm;
int ALARMEN;
int alarm_mode, MATCHED, color;

char buffer[31] = " ECE3301L FA21_S2\0";
char *nbr;
char *txt;
char tempC[] = "+25";
char tempF[] = "+77";
char time[] = "00:00:00";
char date[] = "00/00/00";
char alarm_time[] = "00:00:00";
char Alarm_SW_Txt[] = "OFF";
char Fan_Set_Temp_Txt[] = "075F";
char Fan_SW_Txt[] = "OFF"; // text storage for Heater Mode

char array1[21] = {0xa2, 0x62, 0xe2, 0x22, 0x02, 0xc2, 0xe0, 0xa8, 0x90, 0x68, 0x98, 0xb0, 0x30, 0x18, 0x7a, 0x10, 0x38, 0x5a, 0x42, 0x4a, 0x52};
int array2[21] = {RD1, RD1, RD1, CY1, CY1, GR1, BL1, BL1, MA1, WH1, WH1, WH1, WH1, WH1, WH1, WH1, WH1, WH1, WH1, WH1, WH1}; // add more value into this array

char DC_Txt[] = "000"; // text storage for Duty Cycle value
char Volt_Txt[] = "0.00V";
char RTC_ALARM_Txt[] = "0"; //
char RPM_Txt[] = "0000"; // text storage for RPM

char setup_time[] = "00:00:00";
char setup_date[] = "01/01/00";
char setup_alarm_time[] = "00:00:00";
char setup_fan_set_text[] = "075F";

void putch(char c) {
    while (!TRMT);
    TXREG = c;
}

void init_UART() {
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}
// Initialize the ports 

void Do_Init() {
    init_UART(); // Initialize the uart
    Init_ADC();
    OSCCON = 0x70; // Set oscillator to 8 MHz 
    ADCON1 = 0x0E;
    TRISA = 0x01;
    TRISB = 0x07;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    FAN = 0;
    RBPU = 0;
    TMR3L = 0x00;
    T3CON = 0x03;
    I2C_Init(100000);
    DS1621_Init();
    init_INTERRUPT();
    Turn_Off_Fan();
    fan_set_temp = 75;
    KEY_PRESS = 0;
}

void main() {
    Do_Init(); // Initialization  
    Initialize_Screen();
    tempSecond = 0xff;
    while (1) {
        DS3231_Read_Time();
        if (tempSecond != second) {
            tempSecond = second;
            rpm = get_RPM();
            DS1621_tempC = DS1621_Read_Temp();
            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;

            printf("%02x:%02x:%02x %02x/%02x/%02x", hour, minute, second, month, day, year);
            printf(" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);

            printf("RPM = %d  dc = %d\r\n", rpm, duty_cycle);

            Update_Screen();
        }


        if (Nec_ok == 1) {
            Nec_ok = 0;

            INTCONbits.INT0IE = 1; // Enable external interrupt
            INTCON2bits.INTEDG0 = 0; // Edge programming for INT0 falling edge
            printf("Nec_button = %x\r\n", Nec_button);
            found = 0xff;
            for (int j = 0; j < 21; j++) {
                if (Nec_button == array1[j]) {
                    found = j;
                    j = 21;
                }
            }
            if (found == 0xff) {
                printf("Cannot find button \r\n");
                return;
            } else {
                int i = array2[found];
                printf("Button Position = %x\r\n", found);
                KEY_PRESS_LED = 1;
                PORTB = PORTB & 0x07;
                PORTB = (i << 3) | PORTB;
                Activate_Buzzer();
                Wait_One_Second(); 
                Deactivate_Buzzer();
                KEY_PRESS_LED = 0;
                switch (found) {
                    case EQ_Key:
                        DS3231_Setup_Time();
                        break;
                    case Minus_Key:
                        Decrease_Speed();
                        break;
                    case Plus_Key:
                        Increase_Speed();
                        break;
                    case Play_Pause_Key:
                        Toggle_Fan();
                        KEY_PRESS = 1;
                        Wait_One_Second();
                        KEY_PRESS = 0;
                        break;
                    default:
                        Do_Beep_Bad();
                        break;
                }
            }
        }
    }
}