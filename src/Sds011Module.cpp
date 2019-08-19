#include "Sds011Module.h"
#include <ContiUtils.h>
#include <Value/NodeValue.h>

Sds011Module::Sds011Module(Conti& thing, HardwareSerial& serial, uint8_t rxPin, uint8_t txPin) :
	_thing(thing),
	_logger(ContiLoggerInstance),
	_serial(serial),
	_rxPin(rxPin),
	_txPin(txPin)
{
}

const char* Sds011Module::GetName()
{
	return "SDS011";
}

bool Sds011Module::Init()
{
#ifdef ESP32
	_serial.begin(9600, SERIAL_8N1, _rxPin, _txPin, false);
#endif
	_sds011.setup(&_serial);

	_pm25Node = _thing.AddFloat("PM25", ContiUnit::UgPerM3);
	_pm10Node = _thing.AddFloat("PM10", ContiUnit::UgPerM3);

	_sds011.onData([&](float pm25Value, float pm10Value)
	{
		_pm25Node->SetValue(NodeValue::Float(pm25Value));
		_pm10Node->SetValue(NodeValue::Float(pm10Value));
	});
	_sds011.onError([](int8_t error)
	{
		Serial.printf("Error\n");
	});
	return true;
}

bool Sds011Module::Tick()
{
	_sds011.loop();
	return true;
}
