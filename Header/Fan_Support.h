/* File:   Fan_Support.h */
#ifndef FAN_SUPPORT_H
#define	FAN_SUPPORT_H
float get_RPM();
void Toggle_Fan();
void Turn_Off_Fan();
void Turn_On_Fan();
void Increase_Speed();
void Decrease_Speed();
void do_update_pwm(char);
void Set_DC_RGB(int);
void Set_RPM_RGB(int);
void Monitor_Fan();
int get_duty_cycle(int,int);
#endif	/* FAN_SUPPORT_H */