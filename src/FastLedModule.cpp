#include "FastLedModule.h"


void rainbow(CRGB *leds, int count, int gHue)
{
	fill_rainbow(leds, count, gHue, 7);
}

void addGlitter(CRGB *leds, int count, int gHue, fract8 chanceOfGlitter)
{
	if (random8() < chanceOfGlitter) {
		leds[random16(count)] += CRGB::White;
	}
}

void rainbowWithGlitter(CRGB *leds, int count, int gHue)
{
	// built-in FastLED rainbow, plus some random sparkly glitter
	rainbow(leds, count, gHue);
	addGlitter(leds, count, gHue, 80);
}

void confetti(CRGB *leds, int count, int gHue)
{
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, count, 10);
	int pos = random16(count);
	leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sinelon(CRGB *leds, int count, int gHue)
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, count, 20);
	int pos = beatsin16(13, 0, count - 1);
	leds[pos] += CHSV(gHue, 255, 192);
}

void bpm(CRGB *leds, int count, int gHue)
{
	// colored stripes pulsing at a defined Beats-Per-Minute (BPM)
	uint8_t BeatsPerMinute = 62;
	CRGBPalette16 palette = PartyColors_p;
	uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
	for (int i = 0; i < count; i++) { //9948
		leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
	}
}

void juggle(CRGB *leds, int count, int gHue) {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy(leds, count, 20);
	byte dothue = 0;
	for (int i = 0; i < 8; i++) {
		leds[beatsin16(i + 7, 0, count - 1)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}

PatternFunc gPatterns[] = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, nullptr };

