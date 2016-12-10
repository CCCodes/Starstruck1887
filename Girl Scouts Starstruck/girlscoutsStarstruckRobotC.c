#pragma config(Sensor, dgtl1,  bumper,         sensorDigitalIn)
#pragma config(Sensor, dgtl2,  jump,           sensorDigitalIn)
#pragma config(Motor,  port1,           LeftBackWheel, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           LeftFrontWheel, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           StarGrabber,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           LiftRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LiftLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           RightFrontWheel, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          RightBackWheel, tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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
	wait1Msec(1350);
	motor[LiftLeft] = 0;
	motor[LiftRight] = 0;
}

void putDownLift()
{
	motor[LiftLeft] = -127;
	motor[LiftRight] = 127;
	wait1Msec(740);
	motor[LiftLeft] = 0;
	motor[LiftRight] = 0;
}

void smack()
{
	motor[StarGrabber] = 50;
	wait1Msec(200);
	motor[StarGrabber] = -50;
	wait1Msec(200);
	motor[StarGrabber] = 0;
}

task main()
{
	// autonomous
	float timeToMid = 4;
	//motor[StarGrabber] = -50;
	move('B', timeToMid, true);
	//liftStar();
	//motor[StarGrabber] = 50;
	//wait1Msec(800);
	motor[StarGrabber] = 0;
	move('F', 2, false);
	// has pushed star over

	// turn right and smack stars on fence
	if (SensorValue[jump] == 0) // jump is in
	{
		move('R', 1.1, false); // for left side
	}
	else
	{
		move('L', 1.1, false); // for right side
	}
	move('B', 1, false);
	//smack();
	move('B', 1, false);
	//smack();
	move('B', 1, false);
	//smack();
	//putDownLift();

	// driver control
	while (1 == 1)
	{
		if (vexRT[Ch3] != 1)
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

		if (vexRT[Btn5D] == 1)
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

		if (vexRT[Btn6D] == 1)
		{
			motor[StarGrabber] = -50;
		}
		else if (vexRT[Btn6U] == 1)
		{
			motor[StarGrabber] = 50;
		}
		else
		{
			motor[StarGrabber] = 0;
		}
	}
}
