#pragma config(Sensor, in1,    mobileangle,       sensorPotentiometer) // 200 back / 990 down 1 / 1010 down 2 / 1831 up   //1635 back /2245 down / 2480 down / 4095 up
#pragma config(Sensor, in2,    armangle,          sensorPotentiometer) // 2122 is up / 845 right angle / 240 down
#pragma config(Sensor, dgtl1,  jump,           sensorDigitalIn)
#pragma config(Sensor, dgtl2,  bumper,         sensorTouch)
#pragma config(Motor,  port1,           RightBackWheel, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           RightFrontWheel, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           MobileScoop,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           LiftLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           LiftRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           HangLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           ConeGrabberMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           HangRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           LeftFrontWheel, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          LeftBackWheel, tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Opens Debug Stream Window automatically
#pragma DebuggerWindows("DebugStream")

#define HANG_RIGHT_SPEED 127 // this is the up speed direction
#define HANG_LEFT_SPEED -127 // this is the up speed direction

#define GRABBER_CLOSE -60
#define GRABBER_OPEN 60

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(9999999)


//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
// values for controlling autonomous
   //
   // remember to consider .4 sec added every time the wheels move because
   //     move adds a little stop.
   int grabberSlowSpeed = 30;
   int grabberMedSpeed = 60;
   int grabberFastSpeed = 100  ;

   float grabberCloseTime = .25;
   float hangArmUpTime = 4;
   int hangArmUpSpeed = 127;
   char turnPoleDirection = 'R' ;
   char turnOppPoleDirection = 'L' ;

   float turnPoleTime = 0;

   float travelPoleTime = 1.5;
   bool stopAtButtonIndicator = true;
   float backAfterPoleTime = .125;

   float hangArmDownTime = 1.5;
   int hangArmDownSpeed = 60;
   float grabberOpenTime = .25;
   float finalBackTime = .75;
   int conePressureToggle = 0 ; // no cone pressure 8U turns it on and any 8 turns it off
   int mobilePressureToggle = 0; // mobile pressure indicator
void move(char direction, float time, bool useBumper)// time in seconds
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
	if (useBumper) //
	{
		// just in case the sensor is never pushed
		// 1 means pushed

		while (SensorValue[bumper] == 0 && countRunTimeMsec < time * 1000.0)
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

	// and now wait a short time before next function
	wait1Msec(400);
}

void moveWithoutStopping(char direction )
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
}

void stopMobileScoop()
{
		motor[MobileScoop] = 0;
}
void moveMobileScoop (char dir, int speed  ){
	if (dir == 'D'){
		motor[MobileScoop] = speed;
	}
	else  // U = Up
	{
 		motor[MobileScoop] = speed * -1;
	}
}

void moveMobileScoopToMax (char dir, int speed ,   float maxTime ) // max time in seconds
{
	 int countRunTimeMsec = 0;
	 if (dir == 'D')
	{
		motor[MobileScoop] = speed;
		while (SensorValue[mobileangle] > 275 && countRunTimeMsec < maxTime * 1000.0) // mobile scoop back stop
		{
			wait1Msec(10);
			countRunTimeMsec += 10;
		} // stopped because max time exceeded or angle too far
	}
	else // up
	{
		motor[MobileScoop] = speed * -1;
		while (SensorValue[mobileangle] < 1730 && countRunTimeMsec < maxTime * 1000.0) // mobile school up stop
		{
			wait1Msec(10);
			countRunTimeMsec += 10;
		} // stopped because max time exceeded or angle too far
	}
	motor[MobileScoop] = 0;
}

void stopHangArm()
{
	motor[HangLeft] = 0;
	motor[HangRight] = 0;
}
void stopLiftBox()
{
		motor[LiftLeft] = 0;
   	motor[LiftRight] = 0;
}


void moveHangArm(char dir, int speed ){
	if (dir == 'D'){
		motor[HangLeft] = speed;
   	motor[HangRight] = (speed * -1);
  }
  else
  {
    motor[HangLeft] = (speed * -1);
   	motor[HangRight] = speed;
  }

}

void moveLiftBox(char dir, int speed){
	if (dir == 'U'){
		motor[LiftLeft] = speed;
   	motor[LiftRight] = (speed * -1);
  }
  else
  {
    motor[LiftLeft] = (speed * -1);
   	motor[LiftRight] = speed;
  }
}
void openConeGrabber (int speed){
	  motor[ConeGrabberMotor] = (speed * -1);
}
void closeConeGrabber (int speed){

		motor[ConeGrabberMotor] = (speed );
 }
void stopConeGrabber()
{
	motor[ConeGrabberMotor] = 0;
}


void dropcone(int backupmsec, int arm1angle, int armdown1msec, int arm2msec, int coneclosemsec, int armupmsec)
  	//    millisec back up, armangle, maxarmdown, millisec arm down after armangle,
  	//        millisec cone open, time arm up
{
	  moveLiftBox('U', 127);
	    wait1Msec(backupmsec);
	  stopLiftBox();
   //lower arm  till 250 and then 1/2 second
 		moveHangArm('D',127);
		int countRunTimeMsec2 = 0;
  	  while (SensorValue(armangle) > arm1angle && countRunTimeMsec2 < armdown1msec) //
  	  {
  					wait1Msec(10);
			      countRunTimeMsec2 += 10;
      }

      wait1Msec(arm2msec);
     stopHangArm();
    //release
  	openConeGrabber ( grabberMedSpeed);
    wait1Msec (grabberOpenTime * coneclosemsec);
    stopConeGrabber();
    // raise arm back up a bit
 		moveHangArm('U',127);
    wait1Msec(armupmsec);
    stopHangArm();
}
void mobilePick(){
 	//going forward
    int speedf = 127; // was 70
  	motor[LeftFrontWheel] = speedf;
		motor[LeftBackWheel] = -1 * speedf;
		motor[RightFrontWheel] = -1 * speedf;
		motor[RightBackWheel] = speedf;
  	//bring mobile goal in
	  wait1Msec(250);
	  motor[MobileScoop]=127;
		wait1Msec(250);
		motor[LeftFrontWheel] = 0;
		motor[LeftBackWheel] = 0;
		motor[RightFrontWheel] = 0;
		motor[RightBackWheel] = 0;
		motor[MobileScoop]=40; // tension after mobile pick
}
void GSautonomousJustPole()
{
   // the grabber will snap shut for a time and then apply low pressure until it opens
   closeConeGrabber ( grabberFastSpeed);
   wait1Msec (grabberCloseTime * 1000);
   closeConeGrabber ( grabberSlowSpeed); // after closing, this applies constant pressure
   //
   // move the arm up and then stop - eventually make this a potentiometer
   moveHangArm('U', hangArmUpSpeed); // move up now that you grabbed the cone
   wait1Msec (hangArmUpTime * 1000);
   stopHangArm();
   // set left/right
   /**
      if (SensorValue[jump] == 0) // jump is in
		{
			turnPoleDirection = 'L'; // turn left when jump 12 is in
		}
		else
		{
			turnPoleDirection = 'R'; // turn right when jump 12 is out
		}
   // go to the pole, hit it and back up a bit
   move(turnPoleDirection, turnPoleTime, false);  // turn to pole
   **/
   move('F', travelPoleTime, stopAtButtonIndicator);  // get to pole
   move('B', backAfterPoleTime, false);  // back up after hitting pole
   //
   // put the arm down slowly
   moveHangArm('D', hangArmDownSpeed); // move up now that you grabbed the cone
   wait1Msec (hangArmDownTime * 1000);
   stopHangArm();
   // open the grabber quickly and then stop
   openConeGrabber ( grabberMedSpeed);
   wait1Msec (grabberOpenTime * 1000);
   stopConeGrabber();
   // Back up when the cone is released
   move('B', finalBackTime, false);
}
void GSautonomousOnlyPickCone()
{
   // the grabber will snap shut for a time and then apply low pressure until it opens
   closeConeGrabber ( grabberFastSpeed);
   wait1Msec (grabberCloseTime * 1000);
   closeConeGrabber ( grabberSlowSpeed); // after closing, this applies constant pressure
   //

 }
 //the auto here
 void GSautonomousMobileGoal()
{
   // the grabber will snap shut for a time and then apply low pressure until it opens
   GSautonomousOnlyPickCone(); // just grab the cone
   // start arm moving up

   moveHangArm('U', hangArmUpSpeed); // move up now that you grabbed the cone
   moveWithoutStopping('F') // move while the scoop is fixing itself
	 //1635 back /2245 down / 2480 down / 4095 up
	  if ( SensorValue(mobileangle) < 1000 ) // push out to front
	  {
	  	motor[MobileScoop] = -60; // mobile scoop set to scoop
	  	while  (SensorValue(mobileangle) < 1000){};
	  	motor[MobileScoop] = 0;
	  }
   move('F', 1.5, false);  // move while lifting stop arm
	 stopHangArm(); //

	 move('F', 2.5, stopAtButtonIndicator);  // move until you reach the mobile goal
   mobilePick();
   motor[MobileScoop]=70;
   // drop the cone
   dropcone(400, 500, 3000, 800, 1200, 1000) ;
  	   //    millisec back up, armangle, maxarmdown, millisec arm down after armangle,
      	//        millisec cone open, time arm up
		move('F', .2, false);  // Forward  a little
 // back up start
   move('B', 1.25, false);  // back up a little
   //
   // set left/right
     if (SensorValue[jump] == 1) // jump is out
		{
			turnPoleDirection = 'L'; // turn left when jump 12 is out
		}
		else
		{
			turnPoleDirection = 'R'; // turn right when jump 12 is in
		}
		// now set the opposite direction
     if (SensorValue[jump] == 1) // jump is out
		{
			turnOppPoleDirection = 'R'; // turn left when jump 12 is out
		}
		else
		{
			turnOppPoleDirection = 'L'; // turn right when jump 12 is in
		}


	// turn around
    move(turnPoleDirection, 1.75, false);  // turn to go back to goal
	  motor[MobileScoop]=40;
   //
   //
   // go forward a long time // to get over the first thing
	 //
     move('F', 3.65, false);  // long run to the bar
     move(turnPoleDirection, .15, false);
  // and now start wiggling the mobile goal off.
     motor[MobileScoop] = 0;
     	 move('B', .1, false);  // backup first time
	// pull the arms back
	//
   // move mobile arm up - forward // first time hopefully over the bump
      moveMobileScoopToMax ('U',127,.5);
      moveMobileScoopToMax('D',127,.4);
      move('B', .1, false);  // backup first time

	   // move mobile arm up and back // second time trying for 5-point zone
      moveMobileScoopToMax ('U',127,.8);
      move('F', .2, false);
      move('B', .15, false);
      moveMobileScoopToMax('D',127,.5);

      move('B', .2, false);  // backup rest of third time
	   // move mobile arm up and back // third time just drop it
      moveMobileScoopToMax ('U',127,.8);
       move('B', .1, false);
      moveMobileScoopToMax('D',127,.5);

      move('B', .1, false);  // backup rest of second time
}
/**void GSautonomous()
{

      if (SensorValue[jump] == 0) // jump is in
		{
			GSautonomousShort();//does little
		}
		else
		{
			GSautonomousReal(); //if the jump is out
		}
}**/
void pre_auton()
{
// Set bStopTasksBetweenModes to false if you want to keep user created tasks
// running between Autonomous and Driver controlled modes. You will need to
// manage all user created tasks if set to false.
bStopTasksBetweenModes = true;

}

task autonomous()
{
	GSautonomousMobileGoal();
}

task usercontrol()
{
// User control code here, inside the loop, jasmine here is where the joystick code is

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

	if (vexRT[Btn5D] == 1) // 5 Lift  box up and down
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

	if (vexRT[Btn8R] == 1) // 8R opens the conegrabber and 8L closes it and 8U applies small pressure as a toggle
	{
 		motor[ConeGrabberMotor] = -100; // fast open 8R
 		 conePressureToggle = 0 ; //
	}
	else if (vexRT[Btn8L] == 1)
	{

 	motor[ConeGrabberMotor] = 100; // fast close 8L
 		conePressureToggle = 0 ; //
//GSautonomousMobileGoal();
	}
	else if (vexRT[Btn8U] == 1)
	{
			  conePressureToggle = 1;
 			  motor[ConeGrabberMotor] = 30; // very slow close 8U toggle

	}
	else if (vexRT[Btn8D] == 1)
	{
		   motor[ConeGrabberMotor] = -40; // very slow open 8D
			 conePressureToggle = 0 ; //
	}
	else
	{
		if  ( conePressureToggle == 0)
	    {	motor[ConeGrabberMotor] = 0;}
	}

  // control the lift box with the # 6 buttons
	if (vexRT[Btn6U] == 1)
	{
		motor[HangLeft] = HANG_LEFT_SPEED ;
		motor[HangRight] = HANG_RIGHT_SPEED;

	}
	else if (vexRT[Btn6D] == 1)
	{
		motor[HangLeft] = HANG_LEFT_SPEED * -1;
		motor[HangRight] = HANG_RIGHT_SPEED * -1;

	}
	else
	{
		motor[HangLeft] = 0;
		motor[HangRight] = 0;
	}

	  // control the mobile scoop with the 7 up and down button
	if (vexRT[Btn7D] == 1) // towards the motor
	{ 	// 1735 is full back
		// 1102 is straight downdown

	  // 2702 is full outside
		motor[MobileScoop] = 127;
		  mobilePressureToggle  = 0;
	}
	else if (vexRT[Btn7U] == 1) //towards the outside
	{
 		motor[MobileScoop] = -127;
		  mobilePressureToggle  = 0;
	}
	else if (vexRT[Btn7R] == 1) // set to scoop
	{
				  mobilePressureToggle  = 0;
		   //1635 back /2245 down / 2480 down / 4095 up
		    if ( SensorValue(mobileangle) < 861 ) // push out to front
		    {
		    	motor[MobileScoop] = -60;
		    }
		    else if ( SensorValue(mobileangle) > 1010 ) // pull back to inside
		    {
		    	motor[MobileScoop] = 40;
		    }
		    else
		    {
		    	motor[MobileScoop] = 0;
		    }

  }
    else if (vexRT[Btn7L] == 1 ) // pull in mobile goal
  {
   mobilePick();
   mobilePressureToggle  = 1;
   // GSautonomousMobileGoal();
	}
	else
	{
		if (  mobilePressureToggle  == 0) {
		motor[MobileScoop] = 0;}

	}

	// run autonomous when 7R is pushed


	/** coding partner button 6 as a sample
	    just add Xmtr2 to the button name
	if (vexRT[Btn6DXmtr2] == 1) // 6 down will run autonomous
  {
         	GSautonomous() ;
  }
  **/

 //	     	  writeDebugStreamLine(" The mobile lifter value at %d  ", SensorValue(mobileangle));
//         writeDebugStreamLine(" The arm lifter value at %d  ", SensorValue(armangle));
           /**
			     	    clearLCDLine(0);                                  // clear the top VEX LCD line
						    clearLCDLine(1);                                  // clear the bottom VEX LCD line

						    setLCDPosition(0,0);                              // set the VEX LCD cursor the first line, first space
						    displayNextLCDString("Potentiometer arm:");           // display "Potentiometer:" on the top line

						    setLCDPosition(1,0);                              // set the VEX LCD cursor the second line, first space
						    displayNextLCDNumber(SensorValue(armangle)); // display the reading of the potentiometer sensor

						    wait1Msec(50);                                    // wait 50 milliseconds to help display properly
						    setLCDPosition(0,0);                              // set the VEX LCD cursor the first line, first space
						    displayNextLCDString("Potentiometer mobile goal:");           // display "Potentiometer:" on the top line

						    setLCDPosition(1,0);                              // set the VEX LCD cursor the second line, first space
						    displayNextLCDNumber(SensorValue(mobileangle)); // display the reading of the potentiometer sensor

						    wait1Msec(50);                                    // wait 50 milliseconds to help display properly


						    **/
						    }
}
