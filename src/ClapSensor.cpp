#include "ClapSensor.h"

class ClapSensor;
ClapSensor *clapSensorInstance;

void onInterrupt()
{
	clapSensorInstance->Handleinterrupt();
}

ClapSensor::ClapSensor(int microphonePin)
{
	_microphonePin = microphonePin;
	IsEnabled = false;
}

void ClapSensor::Handleinterrupt()
{
	if (!IsEnabled)
	{
		return;
	}
	auto ms = millis();
	if (readings.size() > 100)
	{
		readings.clear();
	}
	readings.push_back(ms);
	int state = digitalRead(_microphonePin);
	//Serial.printf(" ## Interrupt change %d, %d\n", state, ms);
}

void ClapSensor::Init()
{
	clapSensorInstance = this;
	pinMode(_microphonePin, INPUT_PULLUP);
	attachInterrupt(_microphonePin, onInterrupt, CHANGE);
}

bool ClapSensor::CheckClaps(int &clapCount)
{
	clapCount = 0;
	if (readings.size() < 4)
	{
		return false;
	}
	int lastIndex = readings.size() - 1;
	ulong lastReading = readings.at(lastIndex);
	ulong now = millis();
	if (now - lastReading < 500)
	{
		return false;
	}

	int smallDiffNumber = 0;

	while (lastIndex >= 1)
	{
		int diff = readings.at(lastIndex) - readings.at(lastIndex - 1);

		if (diff > 80 || lastIndex == 1)
		{
			if (smallDiffNumber < 2)
			{
				Serial.printf("Small diff too small \n");
				readings.clear();
				return false;
			}
			smallDiffNumber = 0;
			clapCount++;
		}
		else if (diff > 18)
		{
			Serial.printf("diff too large %d \n", diff);

			readings.clear();
			return false;
		}
		smallDiffNumber++;
		lastIndex--;
	}
	readings.clear();

	return true;
}