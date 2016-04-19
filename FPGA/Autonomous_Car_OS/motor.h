/*
 * motor.h
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#ifndef motor_INCLUDED
#define motor_INCLUDED


//#define PWM2_EN (0x80000000 | PWM2_BASE)
//#define PWM2_PERIOD PWM2_EN+1
//#define PWM2_DUTY1 PWM2_EN+2
//#define PWM2_DUTY2 PWM2_EN+3
//#define PWM2_PHASE1 PWM2_EN+4
//#define PWM2_PHASE2 PWM2_EN+5


#define PWM0_EN (0x80000000 | PWM0_BASE)
#define PWM0_PERIOD PWM0_EN+1
#define PWM0_DUTY1 PWM0_EN+2
#define PWM0_DUTY2 PWM0_EN+3
#define PWM0_PHASE1 PWM0_EN+4
#define PWM0_PHASE2 PWM0_EN+5

#define PWM1_EN (0x80000000 | PWM1_BASE)
#define PWM1_PERIOD PWM1_EN+1
#define PWM1_DUTY1 PWM1_EN+2
#define PWM1_DUTY2 PWM1_EN+3
#define PWM1_PHASE1 PWM1_EN+4
#define PWM1_PHASE2 PWM1_EN+5

void servo_setting(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
void motor_setting(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
void setSteeringPerc(double percentage);
void setSpeedPerc(double percentage);

//void setSteering(int);
//
//void tractionMotorSetting(unsigned long, unsigned long, unsigned long, unsigned long, unsigned long, unsigned long);
//
//void setSpeedPerc(short);

#endif
