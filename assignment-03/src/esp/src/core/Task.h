#ifndef __TASK__
#define __TASK__
#include <Arduino.h>

class Task
{
	int myPeriod;
	int timeElapsed;
	bool active;
	int state;
	bool justEntered;

public:
	virtual void init(int period)
	{
		myPeriod = period;
		timeElapsed = 0;
		active = true;
	}

	virtual void tick();

	bool updateAndCheckTime(int basePeriod)
	{
		timeElapsed += basePeriod;
		if (timeElapsed >= myPeriod)
		{
			timeElapsed = 0;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool isActive()
	{
		return active;
	}

	void setActive(bool active)
	{
		this->active = active;
	}

	void setState(int state)
	{
		this->state = state;
		justEntered = true;
	}

	int getState()
	{
		return state;
	}

	long elapsedTimeInState()
	{
		return millis() - timeElapsed;
	}

	void logOnce(const String &message)
	{
		Serial.println(message);
		if (justEntered)
		{
			justEntered = false;
		}
	}
};

#endif