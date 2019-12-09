#ifndef PROJECTION_H
#define PROJECTION_H


#define WHISKER_INDEX 0
#define MAX_BEND 100.0
//#define MAX_BEND 7.07
#define LOWEST_EFFECT_VIB 45


int motor1_pin=5;
int motor2_pin=6;
int motor3_pin=9;
int motor4_pin=10;
int motor5_pin=11;
int motor6_pin=12;


void feather_setup(){
  WiFi.setPins(8,7,4,2);
  pinMode(motor1_pin, OUTPUT);
  pinMode(motor2_pin, OUTPUT);
  pinMode(motor3_pin, OUTPUT);
  pinMode(motor4_pin, OUTPUT);
  pinMode(motor5_pin, OUTPUT);
  pinMode(motor6_pin, OUTPUT);
  analogWrite(motor1_pin,0);
  analogWrite(motor2_pin,0);
  analogWrite(motor3_pin,0);
  analogWrite(motor4_pin,0);
  analogWrite(motor5_pin,0);
  analogWrite(motor6_pin,0);
}



void set_pwm(int motor_pin, int dc){
  
  int pin, cycle= dc/100.0*255;
  if (motor_pin==1){
    pin=5;
  }
  else if(motor_pin==2){
    pin=6;
  }
  else if(motor_pin==3){
    pin=9;
  }
  else if(motor_pin==4){
    pin=10;
  }
  else if(motor_pin==5){
    pin=11;
  }
  else if(motor_pin==6){
    pin=12;
  }
  
  analogWrite(pin,cycle);
}


void set_vibration(float x, float y){
    double alpha=atan2(y,x);
    int act_1, act_2; //activate which motor
    float x1, x2; 
    int dc_1,dc_2; //desired duty cycle

    if(((alpha> 60.0*M_PI /180) && (alpha < 120.0*M_PI/180)) || ((alpha > -120.0*M_PI /180) && (alpha < -60*M_PI/180))){
        //x1 refer to axis 2, x2 refer to axis 3
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
          

    }
    else if((alpha>=0 && alpha<= 60.0 *M_PI/180) || (alpha>= -M_PI && alpha <= -120.0*M_PI/180)){
        //x1 refer to axis 2, x2 refer to axis 3

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
        

    }
    else{
        //x1 refer to axis 2, x2 refer to axis 3

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

    }
    analogWrite(motor1_pin,0);
    analogWrite(motor2_pin,0);
    analogWrite(motor3_pin,0);
    analogWrite(motor4_pin,0);
    analogWrite(motor5_pin,0);
    analogWrite(motor6_pin,0);

    
    dc_1=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x1)+LOWEST_EFFECT_VIB;
    dc_2=((100-LOWEST_EFFECT_VIB)/MAX_BEND)*fabs(x2)+LOWEST_EFFECT_VIB;
    if (dc_1==LOWEST_EFFECT_VIB){
      dc_1=0;
    }
    if (dc_2==LOWEST_EFFECT_VIB){
      dc_2=0;
    }
    set_pwm(act_1,dc_1);
    set_pwm(act_2,dc_2);
    

    Serial.print("Motor :");
    Serial.print(act_1);
    Serial.print("  Duty Cycle:");
    Serial.println(dc_1);
    Serial.print("Motor :");
    Serial.print(act_2);
    Serial.print("  Duty Cycle:");
    Serial.println(dc_2);

}



#endif
