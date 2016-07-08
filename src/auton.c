#pragma config(Motor, motor1, LEFT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop,  encoder)
#pragma config(Motor, motor6, RIGHT_DRIVETRAIN_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor10, LIFT_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Motor, motor11, CLAW_MOTOR, tmotorVexIQ, openLoop, encoder)
#pragma config(Sensor, port4,  GYRO,     sensorVexIQ_Gyro)
#pragma config(Sensor, port3,  COLOR_SENSOR,  sensorVexIQ_Color12Color)
#pragma config(Sensor, port8, LIFT_LIMIT_SWITCH, sensorVexIQ_Touch)
#pragma config(Sensor, port7, DISTANCE_SENSOR,     sensorVexIQ_Distance)
#pragma config(Sensor, port2,  TOUCH_LED,      sensorVexIQ_LED)

void driveWithUltraSonic(int distance, int speed);
void driveForwardWithEncoder(int count, int speed, int sleepTime);
void rotate(int degrees);
void lift(int rotatos);
#define MOTOR_SPEED 75

//autonomous main
task main()
{
	//drives forward until the distance sensor detects that it is 200 mm or closer to the wall
	driveWithUltraSonic(460, MOTOR_SPEED);
	lift(250);
	driveForwardWithEncoder(80, 50, 1000);
	setMotorTarget(CLAW_MOTOR, 360, 50);
	waitUntilMotorStop(CLAW_MOTOR);
	//uses the smart motor as a servo to move the lift motor up
	lift(200);

	rotate(-175);
	driveForwardWithEncoder(955, 50, 2350);
	lift(-300);
}

//rotate the lift motor a certain amount
void lift(int rotatos) {
	resetMotorEncoder(LIFT_MOTOR);
	setServoTarget(LIFT_MOTOR, rotatos);
	sleep(1000);
}

//drive forward using the ultrasonic sensor
void driveWithUltraSonic(int distance, int speed) {
	if(distance > 0) {
		while(getDistanceValue(DISTANCE_SENSOR) < distance) {
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, speed);
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, -speed);
		}
	} else {
		while(getDistanceValue(DISTANCE_SENSOR) > -distance) {
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, - speed);
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, speed);
		}
	}
	setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, 0);
	setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, 0);
}

//drive forward using the motor encoders use sleepTime to wait until the motors
//stop
void driveForwardWithEncoder(int count, int speed, int sleepTime) {
	resetMotorEncoder(LEFT_DRIVETRAIN_MOTOR);
	resetMotorEncoder(RIGHT_DRIVETRAIN_MOTOR);
	setMotorTarget(LEFT_DRIVETRAIN_MOTOR, count, speed);
	setMotorTarget(RIGHT_DRIVETRAIN_MOTOR, -count, -speed);
	sleep(sleepTime);
}

//rotate the gyro clockwise is positive for this method
void rotate(int degrees) {
	resetGyro(GYRO);
	if(degrees < 0) {
		while(getGyroDegrees(GYRO) < -degrees) {
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, -100);
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, -100);
		}
	} else {
		while(getGyroDegrees(GYRO) > -degrees) {
			setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, 100);
			setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, 100);
		}
	}
	setMotorSpeed(LEFT_DRIVETRAIN_MOTOR, 0);
	setMotorSpeed(RIGHT_DRIVETRAIN_MOTOR, 0);
}
