#include "BH1750Module.h"
#include <ContiUtils.h>
#include <Value/NodeValue.h>

Bh1750Module::Bh1750Module(Thingify & thing, TwoWire &wire, uint8_t i2cAddress) :
	_logger(ContiLoggerInstance),
	_i2cAddress(i2cAddress),
	_bh1750(wire, i2cAddress),
	_thing(thing),
	_isInitSuccessfull(false),
	_wire(wire),
	_isSensorPresent(false)
{
}

const char* Bh1750Module::GetName()
{
	return "BH1750";
}

bool Bh1750Module::Init()
{
	_lightNode = _thing.AddRange("bh1750.light", 0, 4000, 1, ThingifyUnit::Lx);
	return true;
}

bool Bh1750Module::Tick()
{
	static SoftTimer timer(500);
	if (!timer.IsElapsed())
	{
		return true;
	}

	if (!_isSensorPresent)
	{		
		if (_bh1750.begin(ModeContinuous, ResolutionMid) &&
			_bh1750.startConversion())
		{
			_logger.debug(LogComponent::Sensor, L("BH1750 plugged in"));
			_isSensorPresent = true;
		}
		
	}
	if (!_isSensorPresent)
	{
		return false;
	}
	uint16_t lux;

	if (!_bh1750.isConversionCompleted())
	{
		_lightNode->SetValue(NodeValue::NullInt());
		return true;
	}

	if (_bh1750.read(lux))
	{
		_lightNode->SetValue(lux);
		return true;
	}
	
	_logger.debug(LogComponent::Sensor, L("BH1750 unplugged"));
	_isSensorPresent = false;
	_lightNode->SetValue(NodeValue::NullInt());
		
	return false;
}
