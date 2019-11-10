#pragma once

#include <map>

#include <Arduino.h>
#include <Thingify.h>
#include "Libraries/OrnoGb405Remote.h"
#include "Libraries/OrnoSmartLivingRemote.h"
#include "Libraries/OrnoGb417PbRemote.h"
#include <IModule.h>

enum class RadioRemoteType
{
	OrnoGb405,
	OrnoGb417,
	OrnoSmartLiving
};
class RadioNodeDescriptor
{
public:
	RadioRemoteType RemoteType;
	uint8_t SwitchNumber;
};
class RadioRemoteModule : public IModule
{	
	std::map<uint16_t, RadioNodeDescriptor> _nodeDescriptors;
	OrnoGb405Remote _ornoRemote;
	OrnoSmartLivingRemote _ornoSmartLvingRemote;
	OrnoGb417PbRemote _orgnoGb417;
	bool OnValueChanged(Node *node) override;
	Thingify& _thing;
	int _txPin;
public:
	Node* AddRadioSwitch(const char* name, RadioRemoteType remoteType, int switchNumber);

	RadioRemoteModule(int txPin, Thingify& device);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

