#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

extern char FAN;
extern char duty_cycle;
extern signed int DS1621_tempF;
extern unsigned char setup_fan_set_temp;

#define OFF     0
#define RED     1
#define GREEN   2
#define YELLOW  3
#define BLUE    4
#define PURPLE  5
#define CYAN    6
#define WHITE   7

char LED[7] = {RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE};
// converted function to float to read RPM, otherwise didn't work
float get_RPM() {
    TMR3L = 0;
    T3CONbits.TMR3ON = 1;
    Wait_One_Sec();
    Wait_One_Sec();
    Wait_One_Sec();
    Wait_One_Sec();

    T3CONbits.TMR3ON = 0;
    float RPS = TMR3L;
    return (RPS * 60.00);
}

void Toggle_Fan() {
    if (FAN) { // check to see if FAN is active
        Turn_Off_Fan(); // turn fan off
    } else
        Turn_On_Fan(); // else, leave fan on
}

void Turn_Off_Fan() {
    FAN = 0;
    FAN_EN = 0;
    FAN_EN_LED = 0;
}

void Turn_On_Fan() {
    FAN = 1;
    do_update_pwm(duty_cycle);
    FAN_EN = 1;
    FAN_EN_LED = 1;
}

void Increase_Speed() {
    if (duty_cycle >= 100) {    // if duty_cycle >= 100, beep
        Do_Beep();
        Do_Beep();
    } else {                    // else increment duty_cycle by 5
        duty_cycle += 5;
    }
    do_update_pwm(duty_cycle);  // update pwm
}

void Decrease_Speed() {
    if (duty_cycle <= 0) {      // if duty_cycle <= 0, beep
        Do_Beep();
        Do_Beep();
    } else {                    // else decrement duty_cycle by 5
        duty_cycle -= 5;
        do_update_pwm(duty_cycle);  // update pwm
    }
}

void do_update_pwm(char duty_cycle) {
    float dc_f;
    int dc_I;
    PR2 = 0b00000100;
    T2CON = 0b00000111;
    dc_f = (4.0 * duty_cycle / 20.0);
    dc_I = (int) dc_f;
    if (dc_I > duty_cycle) dc_I++;
    CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
    CCPR1L = (dc_I) >> 2;
}
// D1 LED
void Set_DC_RGB(int duty_cycle) {
        char num = duty_cycle / 10;                                                 // isolates double digit for single digit to load 
    if (num > 7) num = 7;
    PORTB = PORTB & 0x07;                                                       // Mask PortB to preserve first 3 bits
    num = num << 3;                                                             // Shift number loaded in for RGB values into right spot
    PORTB = PORTB | num;     
//    int num = duty_cycle / 10;      // obtain duty_cycle as single digit
//    if (num > 7) num = 7;           // if greater than 7, num = 7 as 7 is highest value
//    num = num << 3;                 // num to the power of 3 (shift left))
//    PORTB = PORTB & 0x07;           // mask first 3 bits as inputs and rest of bits as output for portB
//    PORTB = PORTB | num;            // display num values on portB
}
// D2 LED
void Set_RPM_RGB(int rpm){
        int num;
    if (rpm == 0) num = 0;                                                      // if rpm = 0, turn LED off
    else num = (rpm / 500) + 1;                                                 // will turn rpm into RGB color code and will truncate
    if (num > 7) num = 7;                                                       // if any decimals are present from int division
    PORTE = num;   
//    if (rpm < 1)                    // if rpm < 1, display blank to portE
//        PORTE = 0x00;               
//    else if (rpm >= 3000)           // if rpm >= 3000, display WHITE to portE
//        PORTE = LED[6];
//    else
//        PORTE = LED[rpm / 500];      // else, display rpm/500 color based on the LED[] to portE
}
void Monitor_Fan(){
    duty_cycle = get_duty_cycle(DS1621_tempF, setup_fan_set_temp);
    printf("temp is %d and set temp is %d\r\n", DS1621_tempF, setup_fan_set_temp);
    printf("duty cycle");
    do_update_pwm(duty_cycle);
    if(FAN == 1)
    {
        FAN_EN = 1;
    }
    else FAN_EN = 0; 
}

int get_duty_cycle(int ambient_temp, int set_temp){
    int diff_temp;
   
    if (ambient_temp >= set_temp){
        duty_cycle=0;
        printf("duty cycle is 0\n");
    }
    else {
        int diff_temp = (set_temp - ambient_temp);
        printf("calculate diff temp\n");
        printf("diff_temp is %d \r\n",diff_temp);
    }
    
        float diff_temp_float = (float)diff_temp;
        int dc;
        if (diff_temp >= 50){
            duty_cycle=100;
            printf("100\n");
        }
        if (diff_temp >= 35 && diff_temp < 50){             
            dc = (int)(diff_temp_float * 2.00);
            duty_cycle = dc;
            printf("35-50\n");
        }
        if (diff_temp >= 25 && diff_temp < 35){
            dc = (int)(diff_temp_float * 1.5);
            duty_cycle = dc;
            printf("25-35\n");
        }
        if (diff_temp >= 0 && diff_temp < 10){
            dc = (int)(diff_temp_float * 1.00);
            duty_cycle = dc;
            printf("0-10");
        }
    return duty_cycle;
}

void Toggle_Fan_Monitor(){
    if (FAN) { // check to see if FAN is active
        //FAN_EN = 1; // turn on FAN LED
        Toggle_Fan();
    } else{
        Toggle_Fan();
        //FAN_EN = 0; // else, leave fan on
    }
}