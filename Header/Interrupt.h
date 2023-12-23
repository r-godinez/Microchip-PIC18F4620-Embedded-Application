/* File:   Interrupt.h */
#ifndef INTERRUPT_H
#define	INTERRUPT_H
void init_INTERRUPT();
void interrupt  high_priority chkisr(void) ;
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);

//void init_INTERRUPT();
//void interrupt high_priority chkisr(void) ;
//void INT0_isr(void);
//void INT1_isr(void);
//void INT2_isr(void);
//void INT0_ISR();
//void INT1_ISR();
//void INT2_ISR();
//void Enable_INT_Interrupt(void);
//void TIMER1_isr(void);
//void Reset_Nec_State();
#endif	/* INTERRUPT_H */