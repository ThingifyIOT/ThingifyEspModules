#include "Ccs811Module.h"
#include <ContiUtils.h>
#include <Value/NodeValue.h>

Ccs811Module::Ccs811Module(Conti &thing, TwoWire &wire, uint8_t i2cAddress) :
	_i2cAddress(i2cAddress),
	_thing(thing),
	_ccs811(i2cAddress),
	_wire(wire),
	_logger(ContiLoggerInstance),
	_isSensorPresent(false)
{
}

const char * Ccs811Module::GetName()
{
	return "CCS811";
}

bool Ccs811Module::Init()
{
	_co2PpmNode = _thing.AddInt("ccs811.co2", ContiUnit::Ppm);
	_vocPpbNode = _thing.AddInt("ccs811.voc", ContiUnit::Ppb);

	_co2PpmNode->SetValue(NodeValue::NullInt());
	_vocPpbNode->SetValue(NodeValue::NullInt());
	return true;
}

bool Ccs811Module::ApplyCompensation()
{
	Node *humidNode = _thing.FindNode("hdc1080.humidity");
	Node *temperatureNode = _thing.FindNode("hdc1080.temperature");

	if (humidNode == nullptr || temperatureNode == nullptr)
	{
		return false;
	}

	if (humidNode->Value.IsNull() || temperatureNode->Value.IsNull())
	{
		return false;
	}
	float temperature = temperatureNode->Value.AsFloat();
	float humidity = humidNode->Value.AsInt();

	_logger.debug(LogComponent::Sensor, 
		L("CCS811 apply compensation: temperature = %f C, humidity = %f"),
		temperature, humidity);

	_ccs811.setEnvironmentalData(humidity, temperature);
	return true;
}

bool Ccs811Module::Tick()
{
	static SoftTimer timer(1000);
	if (!timer.IsElapsed())
	{
		return true;
	}

	if (!_isSensorPresent)
	{
		if (_ccs811.begin(_wire) == CCS811Core::SENSOR_SUCCESS)
		{
			_connectTime = millis();
			_logger.debug(LogComponent::Sensor, L("CCS811 plugged in"));
			_isSensorPresent = true;
		}
	}
	if (!_isSensorPresent)
	{
		return false;
	}

	if (millis() - _connectTime < 1000)
	{
		return true;
	}	
	
	ApplyCompensation();
	if (_ccs811.readAlgorithmResults() == CCS811Core::SENSOR_SUCCESS)
	{
		uint16_t co2 = _ccs811.getCO2();
		uint16_t voc = _ccs811.getTVOC();
		_co2PpmNode->SetValue(NodeValue::Int(co2));
		_vocPpbNode->SetValue(NodeValue::Int(voc));
		return true;
	}
	_logger.debug(LogComponent::Sensor, L("CCS811 unplugged"));

	_isSensorPresent = false;
	_co2PpmNode->SetValue(NodeValue::NullInt());
	_vocPpbNode->SetValue(NodeValue::NullInt());
	return false;
}
