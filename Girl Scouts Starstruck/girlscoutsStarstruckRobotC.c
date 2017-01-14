#pragma config(Sensor, dgtl1,  bumper,         sensorDigitalIn)
#pragma config(Sensor, dgtl2,  jump,           sensorDigitalIn)
#pragma config(Motor,  port1,           RightBackWheel, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           RightFrontWheel, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           StarGrabberRight, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           LiftRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LiftLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           HangLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           StarGrabberLeft, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           HangRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           LeftFrontWheel, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          LeftBackWheel, tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define HANG_RIGHT_UP -127
#define HANG_RIGHT_DOWN 127
#define HANG_LEFT_UP 127
#define HANG_LEFT_DOWN -127
#define STAR_GRABBER_CLOSE -60
#define STAR_GRABBER_OPEN 60

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(9999999)


//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

void move(char direction, float time, bool useBumper)
{
	switch (direction)
	{
	case 'F':
	  motor[LeftFrontWheel] = 127;
	  motor[LeftBackWheel] = -127;
	  motor[RightFrontWheel] = -127;
	  motor[RightBackWheel] = 127;
	  break;
	case 'B':
	  motor[LeftFrontWheel] = -127;
	  motor[LeftBackWheel] = 127;
	  motor[RightFrontWheel] = 127;
	  motor[RightBackWheel] = -127;
	  break;
	case 'R':
	  motor[LeftFrontWheel] = 127;
	  motor[LeftBackWheel] = -127;
	  motor[RightFrontWheel] = 127;
	  motor[RightBackWheel] = -127;
	  break;
	case 'L':
	  motor[LeftFrontWheel] = -127;
	  motor[LeftBackWheel] = 127;
	  motor[RightFrontWheel] = -127;
	  motor[RightBackWheel] = 127;
	  break;
	default:
	  return;
	}

	int countRunTimeMsec = 0;
	if (useBumper) // use bumper if 0 is passed as the time parameter
	{
		// just in case the sensor is never pushed
	 	// 1 means not pushed
		while (SensorValue[bumper] == 1 && countRunTimeMsec < time * 1000.0)
		{
			wait1Msec(10);
			countRunTimeMsec += 10;
		} // stopped because max time exceeded or bumper value is 1
		// could have used untilTouch http://cdn.robotc.net/pdfs/natural-language/NL_Cortex_Quick.pdf

	} // not bumper
	else
	{
		wait1Msec(time * 1000);
	}

	// time parameter finished so stop
	motor[LeftBackWheel] = 0;
	motor[LeftFrontWheel] = 0;
	motor[RightBackWheel] = 0;
	motor[RightFrontWheel] = 0;

	// and now wait a second before next function
	wait1Msec(1000);
}

void liftStar()
{
	motor[LiftLeft] = 127;
	motor[LiftRight] = -127;
	wait1Msec(1450);
	motor[LiftLeft] = 0;
	motor[LiftRight] = 0;
}

void putDownLift()
{
	motor[LiftLeft] = -127;
	motor[LiftRight] = 127;
	wait1Msec(1000);
	motor[LiftLeft] = 0;
	motor[LiftRight] = 0;
}

void liftHang()
{
	motor[HangLeft] = HANG_LEFT_UP;
	motor[HangRight] = HANG_RIGHT_UP;
	wait1Msec(4500);
	motor[HangLeft] = 0;
	motor[HangRight] = 0;
}

void dropHang()
{
	motor[HangLeft] = HANG_LEFT_DOWN;
	motor[HangRight] = HANG_RIGHT_DOWN;
	wait1Msec(4000);
	motor[HangLeft] = 0;
	motor[HangRight] = 0;
}


void smack()
{
	/** comment out because stargrabber changed configuration
	motor[StarGrabber] = 70;
	wait1Msec(600);
	motor[StarGrabber] = -70;
	wait1Msec(600);
	motor[StarGrabber] = 0;
	**/
}
void GSautonomous()
{
	float timeToMid = 3.5;
//	motor[StarGrabber] = -70;
	move('B', 4, true);
//	motor[StarGrabber] = 0;
	putDownLift();
	move('F', .2, false);
	liftHang();
	dropHang();
//	move('F', 2.7, false);
/**
	float turnToStraighten = .4;
	if (SensorValue[jump] == 0) // jump is in
	{
		move('L', turnToStraighten, false); // for left side
	}
	else
	{
		move('R', turnToStraighten, false); // for r ight side
	}

	move('B', timeToMid, true);
	liftHang();
	dropHang();
	liftStar();
	motor[StarGrabber] = 40;
	move('B', 1, true);
	wait1Msec(500);
	motor[StarGrabber] = 0;
	smack();
	move('F', 2, false);
	// has pushed star over

	// turn right and smack stars on fence
	if (SensorValue[jump] == 0) // jump is in
	{
		move('R', .7, false); // for left side
	}
	else
	{
		move('L', .7, false); // for right side
	}
	move('B', .5, false);
	smack();
	move('B', .5, false);
	smack();
	move('B', .5, false);
	smack();
	//putDownLift();
	**/
}
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
	GSautonomous();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task usercontrol()
{
  // User control code here, inside the loop

  while (true)
  {
		if (vexRT[Ch3] != 1) // Right jostick circle control moves forward and back
		{
			motor[LeftFrontWheel] = vexRT[Ch3];
			motor[LeftBackWheel] = -vexRT[Ch3];
		}
		else
		{
			motor[LeftFrontWheel] = 0;
			motor[LeftBackWheel] = 0;
		}

		if (vexRT[Ch2] != 1) {
			motor[RightBackWheel] = vexRT[Ch2];
			motor[RightFrontWheel] = -vexRT[Ch2];
		}
		else
		{
			motor[RightFrontWheel] = 0;
			motor[RightBackWheel] = 0;
		}

		if (vexRT[Btn5D] == 1) // 5 Lift 2 arms up and down
		{
			motor[LiftLeft] = -127;
			motor[LiftRight] = 127;
		}
		else if (vexRT[Btn5U] == 1)
		{
			motor[LiftLeft] = 127;
			motor[LiftRight] = -127;
		}
		else
		{
			motor[LiftLeft] = 0;
			motor[LiftRight] = 0;
		}

		if (vexRT[Btn6D] == 1) // 6 is open and close of star grabber arm- one arm moves 360 degrees
		{
			motor[StarGrabberRight] = -50;
			motor[StarGrabberLeft] = 50;
			continue;
		}
		else if (vexRT[Btn6U] == 1)
		{
			motor[StarGrabberRight] = 50;
			motor[StarGrabberLeft] = -50;
			continue;
		}
		else
		{
			motor[StarGrabberRight] = 0;
			motor[StarGrabberLeft] = 0;
		}

				// control both hanging motors with button 7 U and D;
		if (vexRT[Btn7U] == 1)
		{
			motor[HangLeft] = HANG_LEFT_UP;
			motor[HangRight] = HANG_RIGHT_UP;
			continue; // so it doesn't hit the else statements that set the motors to 0
		}
		else if (vexRT[Btn7D] == 1)
		{
			motor[HangLeft] = HANG_LEFT_DOWN;
			motor[HangRight] = HANG_RIGHT_DOWN;
			continue; // so it doesn't hit the else statements that set the motors to 0
		}
		else
		{
			motor[HangLeft] = 0;
			motor[HangRight] = 0;
		}

		if (vexRT[Btn7L] == 1) // handles left arm alone using left and right 7
		{
			motor[HangLeft] = HANG_LEFT_UP;
		}
		else if (vexRT[Btn7R] == 1)
		{
			motor[HangLeft] = HANG_LEFT_DOWN;
		}
		else
		{
			motor[HangLeft] = 0;
		}

		if (vexRT[Btn8L] == 1) // handles right arm alone using lefgt and right 8
		{
			motor[HangRight] = HANG_RIGHT_UP;
		}
		else if (vexRT[Btn8R] == 1)
		{
			motor[HangRight] = HANG_RIGHT_DOWN;
		}
		else
		{
			motor[HangRight] = 0;
		}
		// run autonomous when 8Up is pushed

	/**		if (vexRT[Btn8U] == 1)
		{
			GSautonomous();
		}
		**/
		// 8D moves star grabber arm fast
/**		if (vexRT[Btn8D] == 1)
		{
			motor[StarGrabber] = 70;
		}
		else
		{
			motor[StarGrabber] = 0;
		}
		**/
  }
}
