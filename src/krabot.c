#pragma config(Motor, motor1, LEFT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop,  encoder)
#pragma config(Motor, motor6, RIGHT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor10, LIFT_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11, CLAW_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Sensor, port4,  GYRO,     sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  COLOR_SENSOR,  sensorVexIQ_Color12Color)
#pragma config(Sensor, port8, LIFT_LIMIT_SWITCH, sensorVexIQ_Touch)
#pragma config(Sensor, port7, DISTANCE_SENSOR,     sensorVexIQ_Distance)
#pragma config(Sensor, port2,  TOUCH_LED,      sensorVexIQ_LED)

#define DEADZONE 10
#define USE_ARCADE_DRIVE
#define CLAW_SPEED 50
#define LIFT_SPEED 75

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
			int rotationValue = getJoystickValue(ChC);
			int forwardValue = getJoystickValue(ChA);
			setDeadZone(&forwardValue);
			//sets the right motor speed to the inverse of forwardValue - the rotationValue
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, -(forwardValue - rotationValue));
			//sets the left motor speed to forwardValue + rotationValue
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

//task that controls the touch led
task changeTouchLED()
{
	bool toggle = true;
	while(1) {
		if(getTouchLEDValue(TOUCH_LED)) {
			toggle ^= true;
			setTouchLEDColor(TOUCH_LED, toggle ? colorBlue : colorRed);
			sleep(1000);
		}


		wait1Msec(20);
	}

}

//task that controls the lift
task lift()
{
	bool prevToggle = false;
	bool turnLimitSwitchOff = false;
	bool toggleLimitSwitch = false;
	while(true) {
		toggleLimitSwitch = (bool)getJoystickValue(BtnFUp);
		int liftMotorSpeed = inputProcess(getJoystickValue(BtnRUp), getJoystickValue(BtnRDown), LIFT_SPEED);
		if(toggleLimitSwitch && !prevToggle) {
			turnLimitSwitchOff ^= true;
		}
		//doesn't allow the lift to move forward if it's pressing the limit switch
		if(getBumperValue(LIFT_LIMIT_SWITCH) && !turnLimitSwitchOff) {
			if(liftMotorSpeed < 0) {
				liftMotorSpeed = 0;
			}
		}
		prevToggle = toggleLimitSwitch;
		setMotorSpeed(LIFT_MOTOR, liftMotorSpeed);
		wait1Msec(10);
	}
}

//task that controls the claw
task claw()
{
	while(true) {
		int clawMotorSpeed = inputProcess(getJoystickValue(BtnLUp), getJoystickValue(BtnLDown), CLAW_SPEED);
		setMotorSpeed(CLAW_MOTOR, clawMotorSpeed);

		wait1Msec(10);
	}
}



//main task
task main()
{
	//set the touch LED color
	setTouchLEDColor(TOUCH_LED, colorBlue);

	//reset gyro heading
	resetGyro(GYRO);

	//starts the drive task
	startTask(drive);

	//starts the lift task
	startTask(lift);

	//starts the claw task
	startTask(claw);

	//start the touchLED task
	startTask(changeTouchLED);

    //main task doesn't hog the cpu
	while(true) {
		sleep(25);
	}
}
