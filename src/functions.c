

//returns the absoluteValue of value
long absoluteValue(long value)
{
	return value > 0 ? value : -value;
}

//sets the motorValue to 0 if it is less than or equal to the deadonzone
void setDeadZone(long *motorValue)
{
	if(absoluteValue(*motorValue) <= DEADZONE)
		*motorValue = 0;
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
