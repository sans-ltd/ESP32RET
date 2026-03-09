#include <Arduino.h>
#include "config.h"

#ifdef VIRTUALLED_EMULATE_FASTLED
#include <VirtualLED.h>
extern VirtualLEDs virtualLEDs;
static auto& leds = virtualLEDs;
#else
#include <FastLED.h>
extern CRGB leds[A5_NUM_LEDS]; 
#endif

class ActivityLED {
    public:
        ActivityLED(uint8_t pin, boolean fancy, uint32_t onColor, uint32_t offColor) : pin(pin), fancy(fancy), onColor(onColor), offColor(offColor) {
            if (!fancy) pinMode(pin, OUTPUT);   
        }

        void toggle()
        {
            lastToggleTime = millis();
            counter++;
            if (counter >= BLINK_SLOWNESS) {
                counter = 0;
                state = !state;
                if (!fancy) digitalWrite(pin, state);
                else
                {
                    leds[pin] = state ? onColor : offColor;
                }
            }
        }

        void clear()
        {
            if (!fancy) digitalWrite(pin, LOW);
            else leds[pin] = offColor;
            state = false;
            counter = 0;
        }

        void doLoop()
        {
            if (state && (millis() - lastToggleTime > BLINK_TIMEOUT))
            {
                clear();
            }
        }

    private: 
        uint8_t pin;
        bool fancy;
        uint32_t onColor;
        uint32_t offColor;
        bool state = false;
        int counter = 0;
        bool cleared = true;
        uint32_t lastToggleTime = 0;
};