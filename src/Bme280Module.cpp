#include "Bme280Module.h"
#include <ContiUtils.h>
#include <Value/NodeValue.h>

Bme280Module::Bme280Module(Thingify &thing, TwoWire &wire, uint8_t bmeAddress):
	_logger(LoggerInstance),
	_thing(thing),
	_bmeAddress(bmeAddress),
	_bmeSettings(BME280::OSR_X16, BME280::OSR_X16, BME280::OSR_X16,
		BME280::Mode_Normal, BME280::StandbyTime_10ms,
		BME280::Filter_16, BME280::SpiEnable_False, (BME280I2C::I2CAddr)bmeAddress),
	_bme(wire, _bmeSettings),
	_wire(wire),
	_isSensorPresent(false)
{
}

const char * Bme280Module::GetName()
{
	return "BME280";
}

bool Bme280Module::Init()
{	
	_temperatureNode = _thing.AddFloat("bme.temperature", ThingifyUnit::Celsius);
	_humidityNode = _thing.AddRange("bme.humidity",0,100,1, ThingifyUnit::Percent);
	_pressureNode = _thing.AddFloat("bme.pressure", ThingifyUnit::Hectopascal);

	_temperatureNode->SetValue(NodeValue::NullFloat());
	_humidityNode->SetValue(NodeValue::NullInt());
	_pressureNode->SetValue(NodeValue::NullFloat());
	return true;
}
double Bme280Module::Round(double x)
{
	return roundf(x * 10) / 10;
}

bool Bme280Module::Tick()
{
	static SoftTimer timer(500);
	if (!timer.IsElapsed())
	{
		return true;
	}

	if (!_isSensorPresent)
	{
		if (_bme.begin())
		{
			_connectTime = millis();
			_logger.debug(LogComponent::Sensor, L("BME280 plugged in"));
			_isSensorPresent = true;
		}
	}
	if (!_isSensorPresent)
	{
		return false;
	}		

	float temp, humidity, pressure;
	BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
	BME280::PresUnit presUnit(BME280::PresUnit_hPa);

	if (millis() - _connectTime < 500)
	{
		return true;
	}

	if (_bme.read(pressure, temp, humidity, tempUnit, presUnit))
	{
		_temperatureNode->SetValue(NodeValue::Float(Round(temp)));		
		_pressureNode->SetValue(NodeValue::Float(Round(pressure)));
		if (_bme.chipModel() == BME280::ChipModel_BME280)
		{
			_humidityNode->SetValue(NodeValue::Int(Round(humidity)));
		}
		else
		{
			_humidityNode->SetValue(NodeValue::NullInt());
		}
		return true;
	}
	_logger.debug(LogComponent::Sensor, L("BME280 unplugged"));

	_isSensorPresent = false;
	_temperatureNode->SetValue(NodeValue::NullFloat());
	_humidityNode->SetValue(NodeValue::NullInt());
	_pressureNode->SetValue(NodeValue::NullFloat());	
	return false;
}
