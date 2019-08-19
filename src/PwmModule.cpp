#include "PwmModule.h"
#include "../../Conti/src/Conti.h"

PwmModule::PwmModule(Conti &device, TwoWire&wire, uint8_t i2cAddress, float pwmFrequency)
	:_device(device), _pca9685(wire, i2cAddress)
{
	_pwmFrequency = pwmFrequency;
	for (int i = 0; i < PwmOutputCount; i++)
	{
		_requestedPwmValues[i] = 0;
		_actualPwmValues[i] = 0;
	}
}

const char* PwmModule::GetName()
{
	return "Pwm PCA9685";
}

bool PwmModule::Tick()
{
	for (int i = 0; i < PwmOutputCount; i++)
	{
		if (_requestedPwmValues[i] != _actualPwmValues[i])
		{
			_pca9685.setPin(i, _requestedPwmValues[i]);
			_actualPwmValues[i] = _requestedPwmValues[i];
		}
	}
	return true;
}

bool PwmModule::Init()
{
	if(!_pca9685.begin())
	{
		SetError("Failed to initialize PCA9685");
		return false;
	}
	_pca9685.setPWMFreq(_pwmFrequency);
	for (int i = 0; i < PwmOutputCount; i++)
	{
		_pca9685.setPin(i, _actualPwmValues[i]);
	}
	return true;
}

bool PwmModule::Set(int outputNumber, bool value)
{
	_requestedPwmValues[outputNumber] = value ? 4095 : 0;
	return true;
}


bool PwmModule::OnValueChanged(Node *node)
{
	const auto result = _nodeToOutputNumberMap.find(node->integerId());
	if (result == _nodeToOutputNumberMap.end())
	{
		Serial.println("Failed to find matching output number");
		return false;
	}
	const int outputNumber = result->second;

	if (node->Value.Type == ContiType::Bool)
	{
		const auto boolValue = node->Value.AsBool();
		const auto pwmValue = boolValue ? 4095 : 0;
		_requestedPwmValues[outputNumber] = pwmValue;
	}
	else if (node->Type == NodeType::Range)
	{
		const int factor = 4095 / 100.0;
		_requestedPwmValues[outputNumber] = node->Value.AsInt() * factor;
	}

	return true;
}

Node* PwmModule::AddPwmOutput(const char *name, int outputNumber)
{
	const auto node = _device.AddRange(name, 0, 100, 1, ContiUnit::Percent);
	node->OnChanged(this, ValueChangedHandler);
	_nodeToOutputNumberMap[node->integerId()] = outputNumber;
	return node;
}

Node* PwmModule::AddOnOffOutput(const char *name, int outputNumber)
{
	const auto node = _device.AddBoolean(name);
	node->OnChanged(this, ValueChangedHandler);
	_nodeToOutputNumberMap[node->integerId()] = outputNumber;
	return node;
}