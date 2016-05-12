#define LEFT_MOTOR_SPEED motor1
#define RIGHT_MOTOR_SPEED motor2
#define DEADZONE 10

//returns the absolute value of value
long absoluteValue(long value)
{
	if(value > 0)
		return value;
	return -value;
}

//the drive task
task drive()
{
	while(1) {
		int forwardValue = getJoystickValue(ChA);
		int rotationValue = getJoystickValue(ChC);

		setMotorSpeed(LEFT_MOTOR_SPEED, forwardValue + rotationValue);
		setMotorSpeed(RIGHT_MOTOR_SPEED, forwardValue - rotationValue);

		wait10Msec(1);
	}
}

//the main task
task main()
{
	startTask(drive);
}
