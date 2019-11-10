#include "HumidityModule.h"

HumidityModule::HumidityModule(Thingify* thing, uint8_t pin): 
_thing(thing), 
_pin(pin)
{
}

const char* HumidityModule::GetName()
{
	return "Humidity";
}

bool HumidityModule::Init()
{
	_dht = new DHT(_pin, DHT11, 15);
	_dht->begin();
	
	temperatureNode = _thing->AddFloat("dht.temperature", ThingifyUnit::Celsius);
	humidityNode = _thing->AddRange("dht.humidity", 0, 100, 1, ThingifyUnit::Percent);
	return true;
}

bool HumidityModule::Tick()
{
	static SoftTimer soft_timer(5000);
	if(!soft_timer.IsElapsed())
	{
		return true;
	}
	_dht->read();

	const float temperature = _dht->readTemperature();
	const auto humidity = static_cast<int>(_dht->readHumidity());

	temperatureNode->SetValue(NodeValue::Float(temperature));

	if (humidity >= 0 && humidity <= 100)
	{
		humidityNode->SetValue(NodeValue::Int(humidity));
	}
	return true;
}
