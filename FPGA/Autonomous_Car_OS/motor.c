/*
 * motor.c
 *
 *  Created on: 13.05.2015
 *  Author: Ferdinand Goss, Julius Imbery, Markus Steimle
 */

#include "motor.h"
#include <system.h>
#include <stdio.h>

/** Note regarding PWM registers:
 * Phase shifts PWM (nearly never necessary(
 * duty is time PWM is high, 1 unit corresponds to 20 ns
 * period is time for PWM cycle, 1 unit corresponds to 20 ns
 * enable: 1 = PWM on pin one, 2= PWM on pin two, 3= PWM on both pins
 */


/****************************************************************************
  Function:
    void servo_setting(unsigned long phase1, unsigned long duty1,
			unsigned long phase2, unsigned long duty2, unsigned long period,unsigned long enable)

  Description:
    This function sets the values for the PWM to set the servo motor

  Precondition:
    None

  Parameters:
	unsigned long phase1 - Phase 1
	unsigned long duty1  - Duty 1
	unsigned long phase2 - Phase 2
	unsigned long duty2  - Duty 2
	unsigned long period - Period
	unsigned long enable - Enable

  Returns:
    None

  Remarks:
	This function is called by setSteering(int anglePos) 
 **************************************************************************/
void servo_setting(unsigned long phase1, unsigned long duty1,unsigned long phase2, unsigned long duty2,
		unsigned long period,unsigned long enable)
{
	unsigned long * pwm_en= (unsigned long *)PWM0_EN;
	unsigned long * pwm_period=(unsigned long *)PWM0_PERIOD;
	unsigned long * pwm_phase1=(unsigned long *)PWM0_PHASE1;
	unsigned long * pwm_phase2=(unsigned long *)PWM0_PHASE2;
	unsigned long * pwm_duty1=(unsigned long *)PWM0_DUTY1;
	unsigned long * pwm_duty2=(unsigned long *)PWM0_DUTY2;

	* pwm_en=enable;
	* pwm_period=period;
	* pwm_phase1=phase1;
	* pwm_phase2=phase2;
	* pwm_duty1=duty1;
	* pwm_duty2=duty2;

}

/****************************************************************************
  Function:
    void motor_setting(unsigned long phase1, unsigned long duty1,
			unsigned long phase2, unsigned long duty2, unsigned long period,unsigned long enable)

  Description:
    This function sets the values for the PWM to set the drive motor

  Precondition:
    None

  Parameters:
	unsigned long phase1 - Phase 1
	unsigned long duty1  - Duty 1
	unsigned long phase2 - Phase 2
	unsigned long duty2  - Duty 2
	unsigned long period - Period
	unsigned long enable - Enable

  Returns:
    None

  Remarks:
	This function is called by setSpeedPerc(float percentage)
 **************************************************************************/
void motor_setting(unsigned long phase1, unsigned long duty1,unsigned long phase2, unsigned long duty2,
		unsigned long period,unsigned long enable)
{
	unsigned long * pwm_en= (unsigned long *)PWM1_EN;
	unsigned long * pwm_period=(unsigned long *)PWM1_PERIOD;
	unsigned long * pwm_phase1=(unsigned long *)PWM1_PHASE1;
	unsigned long * pwm_phase2=(unsigned long *)PWM1_PHASE2;
	unsigned long * pwm_duty1=(unsigned long *)PWM1_DUTY1;
	unsigned long * pwm_duty2=(unsigned long *)PWM1_DUTY2;

	* pwm_en=enable;
	* pwm_period=period;
	* pwm_phase1=phase1;
	* pwm_phase2=phase2;
	* pwm_duty1=duty1;
	* pwm_duty2=duty2;

}


/****************************************************************************
  Function:
    void setSpeedPerc(short percentage)

  Description:
    This function sets the speed of the vehicle

  Precondition:
    None

  Parameters:
    float percentage - speed from 0% till 100%

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void setSpeedPerc(double percentage)
{
	const unsigned long steady = 75e3;
	unsigned long dutyadd;




	if (percentage > 100)
	{
		percentage = 100;
	} else if (percentage<7 && percentage>-25){
		// apply no signal between -25 and +7 because is to less torque to move the wheels
		percentage=0;
	} else if (percentage < -100)
	{
		percentage = -100;
	}

	// add between -25000 to +25000
	dutyadd = percentage*25000/100;

	//printf("motor_setting %lu, perc=%f\n",steady+dutyadd,percentage);
	motor_setting(0, steady+dutyadd, 0, 0, 1e6, 1);
}

/****************************************************************************
  Function:
    void setSteeringPerc(float percentage)

  Description:
    This function sets the steering of the vehicle in percentage

  Precondition:
    None

  Parameters:
    float percentage - form -100 (full left) to +100 (full right)

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void setSteeringPerc(double percentage)
{
	const unsigned long steady = 75000;
	unsigned long dutyadd;

	if (percentage > 100)
	{
		percentage = 100;
	}
	else if (percentage < -100)
	{
		percentage = -100;
	}

	// add between -20000 to +20000
	dutyadd = percentage*20000/100;
//	printf("servo_setting %lu, perc=%f\n",steady+dutyadd,percentage);
	servo_setting(0, steady+dutyadd, 0, 0, 1e6, 1);
}

/****************************************************************************
  Function:
    void setSteering(float angle)

  Description:
    This function sets the steering angle of the vehicle

  Precondition:
    None

  Parameters:
    float angle - angle from -17° to +17° (negative for left)

  Returns:
    None

  Remarks:
    None
  ***************************************************************************/
void setSteering(double angle)
{
	const double maxangle = 17;
    setSteeringPerc(angle/maxangle);
}



