#pragma once

#include <IModule.h>
#include <Thingify.h>
#include <WiFiUdp.h>


class WolModule : public IModule
{
	Thingify& _device;
	WiFiUDP _udp;
	const char* _macAddress;
	bool ParseMacAddress(const char*macAddress, byte mac[6]);
	NodeValue OnFunctionExecuted(void* context, FunctionArguments& args) override;
public:
	WolModule(Thingify &device, const char* macAddress);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;

	int HexToInt(char ch);
	
};

