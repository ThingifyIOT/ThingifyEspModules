#include "Hdc1080Module.h"
#include <ContiUtils.h>
#include <Value/NodeValue.h>

Hdc1080Module::Hdc1080Module(Thingify &thing, TwoWire &wire, uint8_t i2cAddress) :
	_logger(LoggerInstance),
	_i2cAddress(i2cAddress),
	_thing(thing),	
	_hdc(wire),
	_wire(wire),
	_isSensorPresent(false)
{
}

const char * Hdc1080Module::GetName()
{
	return "HDC1080";
}

bool Hdc1080Module::Init()
{
	_temperatureNode = _thing.AddFloat("hdc1080.temperature", ThingifyUnit::Celsius);
	_humidityNode = _thing.AddRange("hdc1080.humidity", 0, 100, 1, ThingifyUnit::Percent);

	_temperatureNode->SetValue(NodeValue::NullFloat());
	_humidityNode->SetValue(NodeValue::NullInt());
	return true;
}
double Hdc1080Module::Round(double x)
{
	return roundf(x * 10) / 10;
}

bool Hdc1080Module::Tick()
{
	static SoftTimer timer(500);
	if (!timer.IsElapsed())
	{
		return true;
	}

	if (!_isSensorPresent)
	{
		if (_hdc.begin(_i2cAddress))
		{
			_connectTime = millis();
			_logger.debug(LogComponent::Sensor, L("HDC1080 plugged in"));
			_isSensorPresent = true;
		}
	}
	if (!_isSensorPresent)
	{
		return false;
	}

	double temp, humidity;

	if (_hdc.readTemperature(temp) && _hdc.readHumidity(humidity))
	{
		_temperatureNode->SetValue(NodeValue::Float(Round(temp)));
		_humidityNode->SetValue(NodeValue::Int(Round(humidity)));
		return true;
	}	

	_logger.debug(LogComponent::Sensor, L("BME280 unplugged"));
	_isSensorPresent = false;
	_temperatureNode->SetValue(NodeValue::NullFloat());
	_humidityNode->SetValue(NodeValue::NullInt());
	return false;
}
