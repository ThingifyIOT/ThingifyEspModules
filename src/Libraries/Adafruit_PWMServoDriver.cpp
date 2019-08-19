#include "Adafruit_PWMServoDriver.h"

#define WIRE Wire

Adafruit_PWMServoDriver::Adafruit_PWMServoDriver(TwoWire& wire,uint8_t addr)
:_wire(wire),
_i2caddr(addr)
{
}

bool Adafruit_PWMServoDriver::begin(void)
{
	_wire.beginTransmission(_i2caddr);
	if (_wire.endTransmission() != 0)
	{
		Serial.println("PCA8596 driver not connected");
		return false;
	}
	reset();
	return true;
}


void Adafruit_PWMServoDriver::reset(void)
{
	write8(PCA9685_MODE1, 0x0);
}

bool Adafruit_PWMServoDriver::setPWMFreq(float freq)
{
	freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11).
	float prescaleval = 25000000;
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;
	uint8_t prescale = floor(prescaleval + 0.5);
	uint8_t oldmode = read8(PCA9685_MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10; // sleep
	write8(PCA9685_MODE1, newmode); // go to sleep
	write8(PCA9685_PRESCALE, prescale); // set the prescaler
	write8(PCA9685_MODE1, oldmode);
	delay(5);
	//  This sets the MODE1 register to turn on auto increment.
	// This is why the beginTransmission below was not working.
	return write8(PCA9685_MODE1, oldmode | 0xa1);
}

bool Adafruit_PWMServoDriver::setPWM(uint8_t num, uint16_t on, uint16_t off)
{
	_wire.beginTransmission(_i2caddr);
	_wire.write(LED0_ON_L + 4 * num);
	_wire.write(on);
	_wire.write(on >> 8);
	_wire.write(off);
	_wire.write(off >> 8);
	return _wire.endTransmission() == 0;
}

// Sets pin without having to deal with on/off tick placement and properly handles
// a zero value as completely off.  Optional invert parameter supports inverting
// the pulse for sinking to ground.  Val should be a value from 0 to 4095 inclusive.
bool Adafruit_PWMServoDriver::setPin(uint8_t num, uint16_t val, bool invert)
{
	// Clamp value between 0 and 4095 inclusive.
	if (val > 4095) val = 4095;

	uint16_t on = 0;
	uint16_t off = 0;


	if (invert)
	{
		if (val == 0)
		{
			on = 4096;
			off = 0;
		}
		else if (val == 4095)
		{
			on = 0;
			off = 4096;
		}
		else
		{
			on = 0;
			off = 4095 - val;
		}
	}
	else
	{
		if (val == 4095)
		{
			on = 4096;
			off = 0;
		}
		else if (val == 0)
		{
			on = 0;
			off = 4096;
		}
		else
		{
			on = 0;
			off = val;
		}
	}
	return setPWM(num, on, off);
}

uint8_t Adafruit_PWMServoDriver::read8(uint8_t addr)
{
	_wire.beginTransmission(_i2caddr);
	_wire.write(addr);
	_wire.endTransmission();

	_wire.requestFrom(static_cast<uint8_t>(_i2caddr), static_cast<uint8_t>(1));
	return _wire.read();
}

bool Adafruit_PWMServoDriver::write8(uint8_t addr, uint8_t d)
{
	_wire.beginTransmission(_i2caddr);
	_wire.write(addr);
	_wire.write(d);
	return _wire.endTransmission() == 0;
}
