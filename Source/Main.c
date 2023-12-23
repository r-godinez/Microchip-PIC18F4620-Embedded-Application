#include "Main.h"
#pragma config OSC = INTIO67
#pragma config BOREN =OFF
#pragma config WDT=OFF
#pragma config LVP=OFF

#define OFF          0
#define RD           1
#define GR           2
#define YL  		 3
#define BL           4
#define MA           5
#define CY           6
#define WH           7

void test_alarm();
void do_update_pwm(char);

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

short nec_ok;
char Nec_button;
unsigned char Nec_State=0;
short Nec_ok;
char FAN;
char duty_cycle;
int rps;
int rpm;
int ALARMEN, alarm_mode, MATCHED, color;
int glob_RGB = 0;

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
    /*
    TRISD = 0x20;
    TRISE = 0x00;
    PORTE = 0x00;
     */
    TRISD = 0x20;
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
}
// 33:39
void main(){
    Do_Init();
    Initialize_Screen();
    DS3231_Turn_Off_Alarm();
    DS3231_Read_Alarm_Time();
    test_alarm();
    //57:44
    tempSecond = 0xFF;
    while(1){
        DS3231_Read_Time();
        
        if (tempSecond != second){ // wait for second to change
            tempSecond = second;
            rpm = (int)get_RPM();
            volt = read_volt();
            DS1621_tempC = DS1621_Read_Temp();
            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;
            
            printf("%02x:%02x:%02x %02x/%02x/%02x", hour, minute, second, month, day, year);
            printf(" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);
            printf("alarm = %d match = %d", alarm_mode, MATCHED);
            printf("RPM = %d  dc = %d\r\n", rpm, duty_cycle);
                
            Monitor_Fan();      // control fan
            test_alarm();       // control alarm
            Update_Screen();    // refresh screen
        }
        printf("end of second");
        if (check_for_button_input() == 1){
            //nec_ok = 0;
            printf("Button registered \r\n");
            switch (found){
            //case Ch_Minus:
            case Setup_Time_Key:
                printf("Setup Time Key \r\n");
                Do_Beep_Good();
                Do_Setup_Time();
                break;
            //case Channel:
            case Setup_Alarm_Key:
                printf("Setup Alarm Key \r\n");
                Do_Beep_Good();
                Do_Setup_Alarm_Time();
                break;
            //case Ch_Plus:
            case Setup_Fan_Temp_Key:
                printf("Setup Fan Temp \r\n");
                Do_Beep_Good();
                Setup_Temp_Fan();
                break;
            case Toggle_Fan_Key:
                printf("Toggle Fan Key \r\n");
                Do_Beep_Good();
                Toggle_Fan_Monitor();
                break;
            default:
                Do_Beep_Bad();
                break;
            }
        }
        if (INT1_flag==1){
            INT1_flag=0;
            Toggle_Fan();
            //Toggle_Fan_Monitor();
        }
        if (INT2_flag==1){
        INT2_flag=0;
        if (ALARMEN==0) ALARMEN=1;
        else ALARMEN=0;
        }
    }
}

void test_alarm(){
    // 1
//    alarm_mode = (char)alarm_mode;
//    ALARMEN = (char)ALARMEN;
//    bcd_2_dec(alarm_mode);
//    bcd_2_dec(ALARMEN);
    
    if (alarm_mode == 0 && ALARMEN == 1){
        DS3231_Turn_On_Alarm();
        alarm_mode = 1;
        printf("1");
    }
    if (alarm_mode == 1 && ALARMEN == 0){
        DS3231_Turn_Off_Alarm();
        Deactivate_Buzzer();
        alarm_mode=0;
        PORTE=0x00;
        PORTBbits.RB3=0;
        PORTBbits.RB4=0;
        PORTBbits.RB5=0;
        printf("2");
    }
    if (alarm_mode == 1 && ALARMEN == 1){
        if (!RTC_ALARM_NOT){
            //MATCHED = 1;
            //int num;
            Activate_Buzzer();
            if (volt <= 3.0){
                MATCHED=1;
                int num = glob_RGB;
                PORTB = PORTB & 0x07;
                num = num << 3;
                PORTB = PORTB | num;
                Wait_One_Sec();
                glob_RGB++;
                if(glob_RGB==8){
                    glob_RGB=0;
                }
                printf("3");
            }
            
            if (volt > 3.0){
                MATCHED=0;
                ALARMEN=1;
                alarm_mode=0;
                Deactivate_Buzzer();
                PORTBbits.RB3=0;
                PORTBbits.RB4=0;
                PORTBbits.RB5=0;
                printf("4");
            }
        }
    } 
}