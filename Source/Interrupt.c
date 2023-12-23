#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"
#include "stdio.h"

extern char INT0_flag;
extern char INT1_flag;
extern char INT2_flag;

void INT0_ISR() {
    INTCONbits.INT0IF = 0; // Clear the interrupt flag
    INT0_flag = 1; // set software INT0_flag
    Toggle_Fan();
}

void INT1_ISR() {
    INTCON3bits.INT1IF = 0; // Clear the interrupt flag
    INT1_flag = 1; // set software INT1_flag
    Increase_Speed();
}

void INT2_ISR() {
    INTCON3bits.INT2IF = 0; // Clear the interrupt flag
    INT2_flag = 1; // set software INT2_flag
    Decrease_Speed();
}