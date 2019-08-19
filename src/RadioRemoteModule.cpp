#include "RadioRemoteModule.h"


Node* RadioRemoteModule::AddRadioSwitch(const char* name, RadioRemoteType remoteType, int switchNumber)
{
	const auto node = _thing.AddBoolean(name)->OnChanged(this, ValueChangedHandler);

	RadioNodeDescriptor nodeDescriptor {};
	nodeDescriptor.RemoteType = remoteType;
	nodeDescriptor.SwitchNumber = switchNumber;
	_nodeDescriptors[node->integerId()] = nodeDescriptor;
	return node;
}

RadioRemoteModule::RadioRemoteModule(int txPin, Conti &thing)	:
_ornoRemote(txPin), 
_ornoSmartLvingRemote(txPin), 
_orgnoGb417(txPin),
_thing(thing),
_txPin(txPin)
{
	_ornoRemote.SetCode("00101");
}

const char* RadioRemoteModule::GetName()
{
	return "Radio";
}

bool RadioRemoteModule::Init()
{
	_orgnoGb417.Init();
	pinMode(_txPin, OUTPUT);
	digitalWrite(_txPin, LOW);
	return true;
}

bool RadioRemoteModule::Tick()
{
	return true;
}

bool RadioRemoteModule::OnValueChanged(Node* node)
{
	auto& nodeDescriptor = _nodeDescriptors[node->integerId()];
	const bool requestedState = node->Value.AsBool();
	const uint8_t switchNumber = nodeDescriptor.SwitchNumber;

	switch (nodeDescriptor.RemoteType)
	{
		case RadioRemoteType::OrnoGb405: 
			ContiLoggerInstance.info(L("Set OrnoGb405 [%d] to %d"), switchNumber, requestedState);
			_ornoRemote.Set(switchNumber, requestedState);
			break;
		case RadioRemoteType::OrnoGb417:
			ContiLoggerInstance.info(L("Set OrnoGb417 [%d] to %d"), switchNumber, requestedState);
			_orgnoGb417.Set(switchNumber, requestedState);
			break;
		case RadioRemoteType::OrnoSmartLiving:
			ContiLoggerInstance.info(L("Set OrnoSmarLiving [%d] to %d"), switchNumber, requestedState);
			_ornoSmartLvingRemote.Set(nodeDescriptor.SwitchNumber, requestedState);
			break;
	}

	return true;
}
