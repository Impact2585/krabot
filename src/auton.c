#pragma config(Motor, motor1, LEFT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop,  encoder)
#pragma config(Motor, motor6, RIGHT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor10, LIFT_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11, CLAW_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Sensor, port4,  GYRO,     sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  COLOR_SENSOR,  sensorVexIQ_Color12Color)
#pragma config(Sensor, port8, LIFT_LIMIT_SWITCH, sensorVexIQ_Touch)
#pragma config(Sensor, port7, DISTANCE_SENSOR,     sensorVexIQ_Distance)
#pragma config(Sensor, port2,  TOUCH_LED,      sensorVexIQ_LED)

//autonomous main task
task main()
{
	//drives forward until the distance sensor detects that it is at least 600 mm from the wall
	while(getDistanceValue(DISTANCE_SENSOR) < 600) {
		setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, -50);
		setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, 50);
	}
	setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, 0);
	setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, 0);
	//grab the cube using the claw
	setMotorTarget(CLAW_MOTOR, 360, 50);
	waitUntilMotorStop(CLAW_MOTOR);
	//uses the smart motor as a servo to move the lift motor up
	resetMotorEncoder(LIFT_MOTOR);

	setServoTarget(LIFT_MOTOR, 400);
	waitUntilMotorStop(LIFT_MOTOR);
	setMotorSpeed(LIFT_MOTOR, 0);
    //use gyro to rotate the robot 180 degrees
	resetGyro(GYRO);
	while(getGyroDegrees(GYRO) < 180) {
		setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, -100);
	}
	setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, 0);
}
