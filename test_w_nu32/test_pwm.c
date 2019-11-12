#include "NU32.h"          // constants, functions for startup and UART
#define BUF_SIZE 200

int main(void) {
  NU32_Startup();          // cache on, interrupts on, LED/button init, UART init



  T2CONbits.TCKPS = 2;     // Timer2 prescaler N=4 (1:4)
  PR2 = 1999;              // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
  TMR2 = 0;                // initial TMR2 count is 0
  
  OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
  OC2CONbits.OCM = 0b110;
  OC3CONbits.OCM = 0b110;
  OC4CONbits.OCM = 0b110;
  OC5CONbits.OCM = 0b110;

                
  OC1R = 0; // initialize before turning OC1 on; afterward it is read-only
  OC2R = 0; 
  OC3R = 0; 
  OC4R = 0; 
  OC5R = 0; 
  
  OC1RS = 1000;     // duty cycle = OC1RS/(PR2+1) = 25%       
  OC2RS = 1500;
  OC3RS = 1000;
  OC4RS = 0;
  OC5RS = 0;

  
  T2CONbits.ON = 1;        // turn on Timer2
  
  OC1CONbits.ON = 1;       // turn on OC1
  OC2CONbits.ON = 1;
  OC3CONbits.ON = 1;
  OC4CONbits.ON = 1;
  OC5CONbits.ON = 1;


  
  int p=4;
 
  _CP0_SET_COUNT(0);
  while(_CP0_GET_COUNT() < p* 40000000) {
    ;
  }
  
  OC1RS = 0;            // set duty cycle to 50%
  OC2RS = 1000;
  OC3RS = 1500;
  OC4RS = 1000;

  _CP0_SET_COUNT(0);
  while(_CP0_GET_COUNT() < p* 40000000) {
    ;
  }
  
  OC1RS = 0;            // set duty cycle to 50%
  OC2RS = 0;
  OC3RS = 1000;
  OC4RS = 1500;
  OC5RS = 1000;

  _CP0_SET_COUNT(0);
  while(_CP0_GET_COUNT() < p* 40000000) {
    ;
  }
  
  OC1RS = 1000;            // set duty cycle to 50%
  OC2RS = 0;
  OC3RS = 0;
  OC4RS = 1000;
  OC5RS = 1500;


  return 0;
}

