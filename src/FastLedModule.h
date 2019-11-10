#ifndef _FASTLED_MODULE_H
#define _FASTLED_MODULE_H

#include "IModule.h"
#include "Libraries/FastLED/FastLED.h"
#include <Thingify.h>


typedef void(*PatternFunc)(CRGB*leds, int count, int gHue);

extern PatternFunc gPatterns[];

template<unsigned int TLedCount>
class FastLedModule : public IModule
{
	Thingify& _thing;
	CRGB leds[TLedCount];
	int _speed;
	int _brightness;

	uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
	uint8_t gHue = 0; // rotating "base color" used by many of the patterns
	SoftTimer _stUpdateLed;

public:
	FastLedModule(Thingify& thing):_thing(thing), _stUpdateLed(30)
	{
		FastLED.addLeds<WS2812, 15, GRB>(leds, TLedCount);
	}
	const char* GetName() override
	{
		return "Fast led";
	}

	bool OnValueChanged(Node *node) override
	{
		Serial.printf("Node %s changed value\n", node->name());		
		if (strcmp(node->name(), "strip.speed") == 0)
		{
			_speed = node->Value.AsInt();

			int updateInterval = (double)100 / (double)_speed;
			Serial.printf("updated interval to %d\n", updateInterval);
			_stUpdateLed.UpdateDelay(updateInterval);

		}
		if (strcmp(node->name(), "strip.color") == 0)
		{
			auto color = node->Value.AsColor();
			for (int i = 0; i < TLedCount; i++)
			{
				leds[i].red = color.R;
				leds[i].green = color.G;
				leds[i].blue = color.B;
			}
			_brightness = color.A;

		}
		return true;
	}

	bool Init() override
	{	
		_thing.AddColor("strip.color")->OnChanged(this, ValueChangedHandler)
			->SetValue(NodeValue::Color(0,0,0,0));

		//_thing.AddRange("strip.speed", 0, 30, 1, ContiUnit::Percent)->
		//	OnChanged(this, ValueChangedHandler)->
		//	SetValue(NodeValue::Int(10));


		return true;
	}
	bool Tick() override
	{
		EVERY_N_MILLISECONDS(30)
		{
			FastLED.setBrightness(_brightness);
			FastLED.show();
		}
		return true;
		if (_speed == 0)
		{
			return true;
		}
		
		EVERY_N_MILLISECONDS(_speed)
		{
			gHue++;
		}
		if (_stUpdateLed.IsElapsed())
		{
			gPatterns[gCurrentPatternNumber](leds, TLedCount, gHue);
			FastLED.show();
		}

		
		
		static SoftTimer stChangePattern(10000);
		if (stChangePattern.IsElapsed())
		{
			gCurrentPatternNumber++;
			if (gPatterns[gCurrentPatternNumber] == nullptr)
			{
				gCurrentPatternNumber = 0;
			}
			Serial.printf("switched to pattern = %d\n", gCurrentPatternNumber);

		}


		return true;
	}

	
};

#endif

