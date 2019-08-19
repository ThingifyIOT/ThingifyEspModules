#include "TemperatureModule.h"

#include <ContiUtils.h>
#include <map>
#include <set>

TemperatureModule::TemperatureModule(int pin, Conti* device)
{
	_pin = pin;
	_device = device;
}
bool TemperatureModule::Init()
{
	_oneWire = new OneWire(_pin);
	_ds1820 = new DallasTemperature(_oneWire);
	_ds1820->begin();
	_ds1820->setWaitForConversion(false);
	return true;
}

const char* TemperatureModule::GetName()
{
	return "Temperature";
}

Node* TemperatureModule::FindNodeByName(const char* name)
{
	for (int i = 0; i < _nodes.size(); i++)
	{
		if (strcmp(_nodes[i]->name(), name) == 0)
		{
			return _nodes[i];
		}
	}
	return nullptr;
}

float TemperatureModule::GetTemperature(int sensorIndex) const
{
	return _temperature;
}
bool TemperatureModule::Tick()
{
	static SoftTimer soft_timer(3000);
	if (!soft_timer.IsElapsed())
	{
		return true;
	}
	_ds1820->requestTemperatures();

	std::set<std::string> sensors;

	for (int i = 0; i < _ds1820->getDeviceCount(); i++)
	{

		uint8_t sensorId[8];
		_ds1820->getAddress(sensorId, i);

		char sensorIdStr[100];
		sprintf(sensorIdStr, "temp_%02X%02X%02X", sensorId[0], sensorId[1], sensorId[2]);

		float temperature = _ds1820->getTempC(sensorId);

		float b = temperature * 10.0f;
		temperature = static_cast<int>(b) / 10.0f;

		sensors.insert(sensorIdStr);
	
		auto existingNode = FindNodeByName(sensorIdStr);
		if (existingNode == nullptr)
		{
			existingNode = _device->AddFloat(sensorIdStr, ContiUnit::Celsius);
			_nodes.push_back(existingNode);
			Serial.printf("Found new temperature sensor %s\n", sensorIdStr);
		}

		if (temperature != -127.0f)
		{
			existingNode->SetValue(NodeValue::Float(temperature));
		}
	}
	
	static SoftTimer soft_timer_deletion(10000);
	if (!soft_timer_deletion.IsElapsed())
	{
		return true;
	}
	for (auto i = 0; i < _nodes.size(); i++)
	{
		Node *existingNode = _nodes.at(i);

		if (sensors.find(existingNode->name()) == sensors.end())
		{
			_device->RemoveNode(existingNode->name());
			_nodes.erase(_nodes.begin() + i);
			i--;
			Serial.printf("Detached temperature sensor %s\n", existingNode->name());

		}
	}

	return true;
}