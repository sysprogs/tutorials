#pragma once
#include <esp_err.h>

struct RotaryEncoder
{
	int PinA;
	int PinB;
	int EncoderPosition;
	int LastKnownA;
	int LastKnownB;
};

esp_err_t RotaryEncoder_Init(struct RotaryEncoder* encoder, int pinA, int pinB);