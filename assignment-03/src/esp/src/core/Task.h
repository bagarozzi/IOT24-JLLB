#ifndef __TASK__
#define __TASK__
#include <Arduino.h>

class Task
{
	int myPeriod;
	int timeElapsed;
	long stateTime;
	bool active;
	int state;
	bool justEntered;

public:
	virtual void init(int period)
	{
		myPeriod = period;
		timeElapsed = 0;
		active = true;
		stateTime = 0;
	}

	void setState(int state)
	{
		this->state = state;
		justEntered = true;
		stateTime = millis();
	}

	int getState()
	{
		return state;
	}

	long elapsedTimeInState()
	{
		return millis() - stateTime;
	}

	void logOnce(const String &message)
	{
		Serial.println(message);
		if (justEntered)
		{
			justEntered = false;
		}
	}

	bool isJustEntered()
	{
		return justEntered;
	}
};

#endif