#pragma config(Motor, motor1, LEFT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop,  encoder)
#pragma config(Motor, motor6, RIGHT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor10, LIFT_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11, CLAW_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Sensor, port4,  GYRO,     sensorVexIQ_Gyro)
#pragma config(Sensor, port5,  COLOR_SENSOR,  sensorVexIQ_Color12Color)
#pragma config(Sensor, port8, LIFT_LIMIT_SWITCH, sensorVexIQ_Touch)
#pragma config(Sensor, port9, DISTANCE_SENSOR,     sensorVexIQ_Distance)
#pragma config(Sensor, port12,  TOUCH_LED,      sensorVexIQ_LED)

//autonomous main task
task main()
{
	//drives forward until the distance sensor detects that it is 200 mm or closer to the wall
	while(getDistanceValue(DISTANCE_SENSOR) > 200) {
		setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, 50);
		setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, 50);
	}

	//uses the smart motor as a servo to move the lift motor up
	resetMotorEncoder(LIFT_MOTOR);
	setServoTarget(LIFT_MOTOR, 400);

}
