#include "WolModule.h"
#include <FixedString.h>

WolModule::WolModule(Thingify &device, const char* macAddress)
	:_device(device), _macAddress(macAddress)
{
}

const char* WolModule::GetName()
{
	return "Wol";
}

bool WolModule::Init()
{
	_device.AddFunction("Wake Laptop", FunctionExecutionHandler, this);
	return true;
}

bool WolModule::Tick()
{
	return true;
}
int WolModule::HexToInt(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}
	if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	return -1;
}

bool WolModule::ParseMacAddress(const char *macAddress, byte mac[6])
{
	Serial.println(macAddress);
	if (strlen(macAddress) != 17)
	{
		return false;
	}
	for (int i = 0; i < 6; i++)
	{
		if (i != 5)
		{
			byte separator = macAddress[i*3 + 2];
			if (separator != ':')
			{
				return false;
			}
		}
		auto firstDigit = HexToInt(macAddress[i*3]);
		auto secondDigit = HexToInt(macAddress[i*3+1]);
		if (firstDigit == -1 || secondDigit == -1)
		{
			return false;
		}
		mac[i] = firstDigit * 16 + secondDigit;
	}
	return true;
}
NodeValue WolModule::OnFunctionExecuted(void* context, FunctionArguments& args)
{ 
	byte mac[6];
	if (!ParseMacAddress(_macAddress, mac))
	{
		return NodeValue::String("Failed to parse mac");
	}
	//sending packet at port 9
	_udp.beginPacket(IPAddress(255, 255, 255, 255), 9);

	byte preamble[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	_udp.write(preamble, sizeof preamble);

	for (int i = 0; i < 16; i++)
	{
		_udp.write(mac, sizeof mac);
	}
	_udp.endPacket();
	FixedString<50> str;
	str.appendFormat("Wol sent to %s", _macAddress);
	return NodeValue::String(str.c_str());
}



