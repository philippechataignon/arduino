#include <EEPROM.h>

void setup()
{
    for (int i = 0; i < 256; i++) {
        EEPROM.write(i, i);
    }
}

void loop()
{
}
