#include "Arduino.h"

constexpr double BLINK_DELAY_MS = 500;
constexpr int PIN = 8;          // put 13 to use the onboard Led

int main()
{

    // Needed for uart, clock, timers etc.
    init();

    pinMode(PIN, OUTPUT);

    while (true) {
        digitalWrite(PIN, HIGH);
        delay(BLINK_DELAY_MS);
        digitalWrite(PIN, LOW);
        delay(BLINK_DELAY_MS);
    }
}
