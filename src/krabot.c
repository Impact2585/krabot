#pragma config(Motor, motor1, LEFT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop,  encoder)
#pragma config(Motor, motor6, RIGHT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor10, LIFT_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11, CLAW_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Sensor, port4,  GYRO,     sensorVexIQ_Gyro)
#pragma config(Sensor, port5,  COLOR_SENSOR,  sensorVexIQ_Color12Color)
#pragma config(Sensor, port8, LIFT_LIMIT_SWITCH, sensorVexIQ_Touch)

#define DEADZONE 10
#define USE_ARCADE_DRIVE
#define CLAW_SPEED 50

//returns the absoluteValue of value
long absoluteValue(long value)
{
	return value > 0 ? value : -value;
}

//sets the motorValue to 0 if it is less than or equal to the deadonzone
void setDeadZone(long *motorValue)
{
	if(absoluteValue(*motorValue) <= DEADZONE){
		*motorValue = 0;
	}
}

//returns speed if button1 is pressed, -speed if butt
int inputProcess(int buttonValue1, int buttonValue2, int speed)
{
	if(buttonValue1 == 1 && buttonValue2 == 0) {
		return speed;
	} else if(buttonValue1 == 0 && buttonValue2 == 1) {
		return -speed;
	} else {
		return 0;
	}
}

// returns 1 if color sensor detects red 0 otherwise
bool redDetected(){
	return getColorName(COLOR_SENSOR) == colorRed;
}

// returns 1 if color sensor detects blue 0 otherwise
bool blueDetected(){
	return getColorName(COLOR_SENSOR) == colorBlue;
}

// returns value from 0 - 359 for heading of gyro in degrees
long gyroHeading(){
	return getGyroHeading(GYRO);
}

//the drive task
task drive()
{
	while(true) {
		//uses arcade drive if USE_ARCADE_DRIVE is defined and tank drive if not
		#ifdef USE_ARCADE_DRIVE
			//arcade drive
			int rotationValue = getJoystickValue(ChA);
			int forwardValue = getJoystickValue(ChC);
			setDeadZone(&forwardValue);
			//sets the left motor speed to the forwardValue + the rotationValue
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, forwardValue - rotationValue);
			//sets the right motor speed to forwardValue - rotationValue
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, forwardValue + rotationValue);
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
			if(liftMotorSpeed < 0) {
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
	//reset gyro heading
	resetGyro(GYRO);
	
	//starts the drive task
	startTask(drive);

	//starts the lift task
	startTask(lift);

	//starts the claw task
	startTask(claw);

	while(true) {
		sleep(25);
	}
}
