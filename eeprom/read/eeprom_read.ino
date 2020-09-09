#include <EEPROM.h>

void setup()
{
    Serial.begin(9600);

    for (int i = 0; i < 256; i++) {
        byte valeur = EEPROM.read(i);
        Serial.print(i);
        Serial.print(" = ");
        Serial.println(valeur);
    }
}

void loop()
{
}
