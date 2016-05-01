#define LEFT_DRIVETRAIN_MOTOR motor1
#define RIGHT_DRIVETRAIN_MOTOR motor2
#define DEADZONE 10
#define USE_ARCADE_DRIVE

//returns the absoluteValue of value
long absoluteValue(long value)
{
	return value > 0 ? value : -value;
}

//sets the motorValue to 0 if it is less than or equal to the deadonzone
void setDeadzone(long *motorValue)
{
	if(absoluteValue(*motorValue) <= DEADZONE)
		*motorValue = 0;
}

//the drive task
task drive()
{
	while(true) {
		//uses arcade drive if USE_ARCADE_DRIVE is defined and tank drive if not
		#ifdef USE_ARCADE_DRIVE
			//arcade drive
			int forwardValue = getJoystickValue(ChA);
			int rotationValue = getJoystickValue(ChC);
			setDeadzone(&forwardValue);
			//sets the left motor speed to the forwardValue + the rotationValue
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, forwardValue + rotationValue);
			//sets the right motor speed to forwardValue - rotationValue
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, forwardValue - rotationValue);
		#else
			//tank drive
			int leftMotorValue = getJoystickValue(ChA);
			int rightMotorValue = getJoystickValue(ChD);
			setDeadzone(&leftMotorValue);
			setDeadzone(&rightMotorValue);
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, leftMotorValue);
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, rightMotorValue);
		#endif
		//waits 10 milliseconds after every loop to not hog the cpu
		wait1Msec(10); 
	}
}

//main task
task main()
{
	//starts the drive task
	startTask(drive);
}
