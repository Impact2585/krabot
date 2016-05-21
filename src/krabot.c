#include "krabot.h"
#include "functions.c"

//the drive task
task drive()
{
	while(true) {
		//uses arcade drive if USE_ARCADE_DRIVE is defined and tank drive if not
		#ifdef USE_ARCADE_DRIVE
			//arcade drive
			int forwardValue = getJoystickValue(ChA);
			int rotationValue = getJoystickValue(ChC);
			setDeadZone(&forwardValue);
			//sets the left motor speed to the forwardValue + the rotationValue
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, forwardValue + rotationValue);
			//sets the right motor speed to forwardValue - rotationValue
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, forwardValue - rotationValue);
		#else
			//tank drive
			int leftMotorValue = getJoystickValue(ChA);
			int rightMotorValue = getJoystickValue(ChD);
			setDeadZone(&leftMotorValue);
			setDeadZone(&rightMotorValue);
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, leftMotorValue);
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, rightMotorValue);
		#endif
		//waits 10 milliseconds after every loop to not hog the cpu
		wait1Msec(10);
	}
}

//task that controls the lift
task lift()
{
	while(true) {
		int liftMotorSpeed = getJoystickValue(ChD);

		//doesn't allow the lift to move forward if it's pressing the limit switch
		if(getBumperValue(LIFT_LIMIT_SWITCH) == 1) {
			if(liftMotorSpeed > 0) {
				liftMotorSpeed = 0;
			}
		}

		setMotorSpeed(LIFT_MOTOR, liftMotorSpeed);
		wait1Msec(10);
	}
}

//task that controls the claw
task claw()
{
	while(true) {
		int clawMotorSpeed = inputProcess(getJoystickValue(BtnFUp), getJoystickValue(BtnFDown), CLAW_SPEED);
		setMotorSpeed(CLAW_MOTOR, clawMotorSpeed);

		wait1Msec(10);
	}
}

//main task
task main()
{
	//starts the drive task
	startTask(drive);

	//starts the lift task
	startTask(lift);

	//starts the claw task
	startTask(claw);
}
