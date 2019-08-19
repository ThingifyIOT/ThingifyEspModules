#ifndef _ORNO_GB417PB_REMOTE_H
#define _ORNO_GB417PB_REMOTE_H
#include "RcSwitchPatched/RCSwitch.h"

class OrnoGb417PbRemote
{
	RCSwitch _rcSwitch;
	const char *_onCodes[4] = { "F110XFX0FF10", "F11XX11F00FF", "F1F01F10X01X", "F1XFFFF11FF1" };
	const char *_offCodes[4] = { "F1FX0XFFX110", "F1XFFFF11FFF", "F1FFXXX10F1X", "F1XXFX111XF1" };
	int _txPin;

public:
	OrnoGb417PbRemote(int txPin);

	void Init();
	void TurnOn(int outputNumber);
	void TurnOff(int outputNumber);
	void Set(int ouputNumber, bool value);
};

#endif

