#include "DisplayModule.h"
#include <Wire.h>

DisplayModule::DisplayModule(Thingify &thing, int ssd1306Address, int sdaPin, int sclPin, bool isInverted) :
display(ssd1306Address, sdaPin, sclPin),
_scl_pin(sclPin),
_sda_pin(sdaPin),
_thing(thing),
_ssd1306_address(ssd1306Address),
_display_timer(1000),
_isInverted(isInverted)
{
	_thing.OnStateChanged = bind(&DisplayModule::OnStateChanged, this, std::placeholders::_1);
}

void DisplayModule::OnStateChanged(ThingState state)
{
	_display_timer.SetElapsed();
}
bool DisplayModule::Init()
{	
	// 128 x 64 display
	if (!display.init())
	{
		Serial.println("SSD1306 display not connected");
		return false;
	}

	display.setFont(ArialMT_Plain_16);
	display.clear();
	display.setBrightness(50);
	Display();
	return true;
}
void DisplayModule::Display()
{
	if (_isInverted)
	{
		display.flipScreenVertically();
	}	
	display.display();	
}
bool DisplayModule::Tick()
{
	if (!_display_timer.IsElapsed())
	{
		return true;
	}
	DisplayInfo();
	return true;
}

void DisplayModule::DisplayInfo()
{
	display.clear();
	display.setFont(ArialMT_Plain_16);
	display.drawString(1,2, ThingifyUtils::ThingStateToShortStr(_thing.GetCurrentState()));

	if (_thing.GetCurrentState() == ThingState::Online)
	{
		display.setFont(ArialMT_Plain_10);
		FixedString<100> onlineTimeStr;
		auto connectedString = ThingifyUtils::TimeToShortStr(_thing.GetMillisecondsSinceConnect() / 1000);
		onlineTimeStr.append("for ");
		onlineTimeStr += connectedString;

		FixedString<100> uptimeStr = "up ";
		auto uptime = ThingifyUtils::TimeToShortStr(millis() / 1000);
		uptimeStr += uptime;


		display.drawString(62, 0, onlineTimeStr.c_str());
		display.drawString(62, 10, uptimeStr.c_str());


		FixedString<50> packetCountStr;
		packetCountStr.appendFormat("In/Out pkts: %d/%d", _thing.GetIncomingPackets(), _thing.GetOutgoingPackets());
		display.drawString(1, 25, packetCountStr.c_str());

	}
	

	if (_thing.GetCurrentState() == ThingState::ConnectingToNetwork)
	{
		display.drawString(1, 25, _thing.GetNetworkName().c_str());
	}

	if (_thing.UpdateService().IsUpdating())
	{
		display.setColor(OLEDDISPLAY_COLOR::BLACK);
		display.fillRect(5, 5, 128 - 10, 64 - 10);
		display.setColor(OLEDDISPLAY_COLOR::WHITE);
		display.drawRect(5, 5, 128 - 10, 64 - 10);
		display.setColor(OLEDDISPLAY_COLOR::WHITE);
		display.drawProgressBar(10, 22, 128 - 20, 10, _thing.UpdateService().UpdateProgress());
		display.setFont(ArialMT_Plain_10);
		display.drawString(10, 64 - 28, "Upgrading firmware...");
	}

	Display();
}

const char* DisplayModule::GetName()
{
	return "Display";
}
