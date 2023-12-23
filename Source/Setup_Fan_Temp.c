/*  File:   Setup_Fan_Temp.h */
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Main.h"
#include "I2C_Support.h"
#include "Setup_Fan_Temp.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"

extern char setup_fan_set_text[];
unsigned char setup_fan_set_temp;

char Select_Fan_Temp_Field;
char In_Fan_Temp_Setup;

extern char *txt;
extern char found;
extern unsigned char fan_set_temp;
extern short nec_ok;

void Setup_Temp_Fan(void) {
    In_Fan_Temp_Setup = 1;
    setup_fan_set_temp = fan_set_temp;
    Initialize_Setup_Fan_Screen(); // Initialize setup heater screen
    Update_Setup_Fan_Screen(); // Update the screen with latest info

    while (In_Fan_Temp_Setup == 1) {
        if (check_for_button_input() == 1) {
            nec_ok = 0;

            if ((found == Ch_Minus) || (found == Ch_Plus) || (found == Minus) || (found == Plus)) {
                // add code to handle 'Plus', 'Minus', 'Ch_Plus' and 'Ch_Minus'
                Do_Beep_Good();
                if (found == Ch_Plus) Do_Save_New_Fan_Temp();
                if (found == Ch_Minus) Exit_Setup_Fan_Temp();
                if (found == Minus) Decrease_Fan_Temp();
                if (found == Plus) Increase_Fan_Temp();
                found = 0xff;
                } else {
                Do_Beep_Bad();
            }
        }
    }
}

void Increase_Fan_Temp() {
    setup_fan_set_temp++;
    if (setup_fan_set_temp >= 110){
        setup_fan_set_temp = 110;
    }
    // add code to increase setup_fan_set_temp by 1
    // then check if it is greater than 110. If so, limit to 110
    Update_Setup_Fan_Screen(); // Update screen with latest info
}

void Decrease_Fan_Temp() {
            setup_fan_set_temp--;
            if (setup_fan_set_temp <= 50){
                setup_fan_set_temp = 50;
            }
    // add code to decrease setup_fan_set_temp by 1
    // then check if it is less than 50. If so, limit to 50
    Update_Setup_Fan_Screen(); // Update screen with latest info
}

void Exit_Setup_Fan_Temp() {
    DS3231_Read_Time(); // Read time
            Initialize_Screen(); // Initialize the screen before returning  
            In_Fan_Temp_Setup = 0;
}

void Do_Save_New_Fan_Temp() {
    fan_set_temp = setup_fan_set_temp;
            DS3231_Read_Time(); // Read time
            Initialize_Screen(); // Initialize the screen before returning  
            In_Fan_Temp_Setup = 0;
}

void Initialize_Setup_Fan_Screen(void) {
    fillScreen(ST7735_BLACK); // Fills background of screen with color passed to it

            strcpy(txt, "ECE3301L F23-L13_S2\0"); // Text displayed 
            drawtext(start_x, start_y, txt, ST7735_WHITE, ST7735_BLACK, TS_1); // X and Y coordinates of where the text is to be displayed

            strcpy(txt, " Fan Setup\0"); // Text displayed 
            drawtext(start_x, start_y + 25, txt, ST7735_YELLOW, ST7735_BLACK, TS_2);

            strcpy(txt, "  Set Fan Temp");
            drawtext(setup_fan_x, setup_fan_y, txt, ST7735_CYAN, ST7735_BLACK, TS_1);
}

void Update_Setup_Fan_Screen(void) {
    char dc_char1 = setup_fan_set_temp / 100;
    char dc_char2 = (setup_fan_set_temp / 10)%10; 
    char dc_char3 = (setup_fan_set_temp % 10);  
    setup_fan_set_text[0] = dc_char1 + '0';
    setup_fan_set_text[1] = dc_char2 + '0';
    setup_fan_set_text[2] = dc_char3 + '0';
    drawtext(setup_data_fan_x, setup_data_fan_y, setup_fan_set_text, ST7735_RED, ST7735_BLACK, TS_2);
}