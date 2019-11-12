#include "NU32.h"          // constants, functions for startup and UART
#include <xc.h>
#include <math.h>

#define BUF_SIZE 200
#define MAX_BEND 7.07
#define LOWEST_EFFECT_VIB 50

void set_vibration(float x, float y);
void set_pwm(int motor, int dc);
void clear_pwm();

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
    
  T2CONbits.ON = 1;        // turn on Timer2
  
  OC1CONbits.ON = 1;       // turn on OC1
  OC2CONbits.ON = 1;
  OC3CONbits.ON = 1;
  OC4CONbits.ON = 1;
  OC5CONbits.ON = 1;

  char buffer[BUF_SIZE];

  while(1){
       float x,y;
       NU32_ReadUART3(buffer,BUF_SIZE);  
       sscanf(buffer,"%f",&x);
       NU32_ReadUART3(buffer,BUF_SIZE);
       sscanf(buffer,"%f",&y);
       sprintf(buffer,"x: %f , y: %f \r\n",x,y);
       NU32_WriteUART3(buffer);
       
       clear_pwm();

       set_vibration(x,y);
                    
  }
  return 0;
}

void set_vibration(float x, float y){

    char buffer[BUF_SIZE];
    double alpha=atan2(y,x);
    sprintf(buffer,"alpha: %f \r\n",alpha);
    NU32_WriteUART3(buffer);

    if(((alpha> 60.0*M_PI /180) && (alpha < 120.0*M_PI/180)) || ((alpha > -120.0*M_PI /180) && (alpha < -60*M_PI/180))){
        int act_1, act_2; //activate which motor
        float x1, x2; //x1 refer to axis 2, x2 refer to axis 3
        int dc_1,dc_2; //desired duty cycle
        x2 = y / sqrt(3.0) - x;
        x1= 2* x +x2;
       
        if(x1>=0){
            act_1=2;
        }
        else{
            act_1=5;
        }
        if(x2>=0){
            act_2=3;
        }
        else{
            act_2=6;
        }

        dc_1=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x1)+LOWEST_EFFECT_VIB;
        dc_2=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x2)+LOWEST_EFFECT_VIB;

        set_pwm(act_1,dc_1);
        set_pwm(act_2,dc_2);

        sprintf(buffer," activating motor %d and %d with duty cycle  %d %% and %d %% \r\n",act_1,act_2,dc_1,dc_2);
        NU32_WriteUART3(buffer);
    }
    else if((alpha>=0 && alpha<= 60.0 *M_PI/180) || (alpha>= -M_PI && alpha <= -120.0*M_PI/180)){
        int act_1, act_2; //activate which motor
        float x1, x2; //x1 refer to axis 2, x2 refer to axis 3
        int dc_1,dc_2; //desired duty cycle
        x2 = 2.0*y / sqrt(3.0);
        x1= x - x2* 0.5;
       
        if(x1>=0){
            act_1=1;
        }
        else{
            act_1=4;
        }
        if(x2>=0){
            act_2=2;
        }
        else{
            act_2=5;
        }

        dc_1=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x1)+LOWEST_EFFECT_VIB;
        dc_2=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x2)+LOWEST_EFFECT_VIB;
        
        set_pwm(act_1,dc_1);
        set_pwm(act_2,dc_2);
        
        sprintf(buffer," activating motor %d and %d with duty cycle  %d %% and %d %% \r\n",act_1,act_2,dc_1,dc_2);
        NU32_WriteUART3(buffer);

    }
    else{
        int act_1, act_2; //activate which motor
        float x1, x2; //x1 refer to axis 2, x2 refer to axis 3
        int dc_1,dc_2; //desired duty cycle
        x2 = 2.0*y / sqrt(3.0);
        x1= x + x2* 0.5;
       
        if(x1>=0){
            act_1=1;
        }
        else{
            act_1=4;
        }
        if(x2>=0){
            act_2=3;
        }
        else{
            act_2=6;
        }

        dc_1=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x1)+LOWEST_EFFECT_VIB;
        dc_2=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x2)+LOWEST_EFFECT_VIB;
        
        set_pwm(act_1,dc_1);
        set_pwm(act_2,dc_2);
        
        sprintf(buffer," activating motor %d and %d with duty cycle  %d %% and %d %% \r\n",act_1,act_2,dc_1,dc_2);
        NU32_WriteUART3(buffer);
    }

}
void set_pwm(int motor, int dc){
    switch (motor)
    {
    case 1:
        OC1RS = dc/100.0 * (PR2 +1);
        break;
    case 2:
        OC2RS = dc/100.0 * (PR2 +1);
        break;
    case 3:
        OC3RS = dc/100.0 * (PR2 +1);
        break;
    case 4:
        OC4RS = dc/100.0 * (PR2 +1);
        break;
    case 5:
        OC5RS = dc/100.0 * (PR2 +1);
        break;
    // case 6:
    //     OC6RS = dc/100.0 * (PR2 +1);
    //     break;
    default:
        break;
    }

}
void clear_pwm(){
  OC1RS = 0;     
  OC2RS = 0;
  OC3RS = 0;
  OC4RS = 0;
  OC5RS = 0;

}