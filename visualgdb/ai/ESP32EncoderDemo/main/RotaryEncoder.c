#include "RotaryEncoder.h"
#include "driver/gpio.h"
#include "esp_err.h"

static void RotaryEncoder_HandleGPIOInterrupt(void *arg);

esp_err_t RotaryEncoder_Init(struct RotaryEncoder* encoder, int pinA, int pinB)
{
	// Store the pin numbers in the encoder structure
	encoder->PinA = pinA;
	encoder->PinB = pinB;

	// ------------------------------------------------------------
	// GPIO configuration: set the two pins as INPUT with pull‑up
	// ------------------------------------------------------------
	const gpio_config_t io_conf = {
		.pin_bit_mask = ( (1ULL << pinA) | (1ULL << pinB) ),   // selected pins
		.mode = GPIO_MODE_INPUT,                         // Input mode
		.pull_up_en = GPIO_PULLUP_ENABLE,                // Enable internal pull‑up
		.pull_down_en = GPIO_PULLDOWN_DISABLE,           // Disable pull‑down
		.intr_type = GPIO_INTR_ANYEDGE                  // No interrupt (will be added per‑pin)
	};

	esp_err_t err = gpio_config(&io_conf);
	if (err != ESP_OK) {
		return err;
	}

	// ------------------------------------------------------------
	// Install ISR service (if not already installed) and attach handlers
	// ------------------------------------------------------------
	err = gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
	if (err != ESP_OK) {
		return err;
	}

	// Register the same handler for both pins; the encoder pointer is passed as argument
	err = gpio_isr_handler_add((gpio_num_t)pinA, RotaryEncoder_HandleGPIOInterrupt, (void*)encoder);
	if (err != ESP_OK) {
		return err;
	}
	err = gpio_isr_handler_add((gpio_num_t)pinB, RotaryEncoder_HandleGPIOInterrupt, (void*)encoder);
	if (err != ESP_OK) {
		return err;
	}

	return ESP_OK;
}

static void RotaryEncoder_HandleGPIOInterrupt(void *arg)
{
	// Cast argument back to encoder structure
	struct RotaryEncoder *enc = (struct RotaryEncoder *)arg;

	// Read current levels of both pins
	int currentA = gpio_get_level((gpio_num_t)enc->PinA);
	int currentB = gpio_get_level((gpio_num_t)enc->PinB);

	// Detect edges by comparing with last known states
	int edgeA = currentA - enc->LastKnownA; // +1 for positive edge, -1 for negative edge, 0 for no change
	int edgeB = currentB - enc->LastKnownB; // +1 for positive edge, -1 for negative edge, 0 for no change

	// Apply quadrature decoding logic
	if (edgeA != 0) {
		// Edge detected on A
		if ((edgeA > 0 && currentB == 1) || (edgeA < 0 && currentB == 0)) {
			// Positive edge on A with B=1 or negative edge on A with B=0 => increment
			enc->EncoderPosition++;
		} else {
			// All other A transitions => decrement
			enc->EncoderPosition--;
		}
	}

	if (edgeB != 0) {
		// Edge detected on B
		if ((edgeB > 0 && currentA == 0) || (edgeB < 0 && currentA == 1)) {
			// Positive edge on B with A=0 or negative edge on B with A=1 => increment
			enc->EncoderPosition++;
		} else {
			// All other B transitions => decrement
			enc->EncoderPosition--;
		}
	}

	// Update last known states
	enc->LastKnownA = currentA;
	enc->LastKnownB = currentB;
}
