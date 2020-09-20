#define PAUSE 1000

byte mask[] = {
    0x3F,
    0x06,
    0x5B,
    0x4F,
    0x66,
    0x6D,
    0x7D,
    0x07,
    0x7F,
    0x6F,
    0x77,
    0x7C,
    0x39,
    0x5E,
    0x79,
    0x71
};

void setup()
{
    for (int i = 1; i < 8; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop()
{
    for (int i = 1; i < 8; i++) {
        digitalWrite(i, LOW);
    }
    for (int i = 0; i < 7; i++) {
        digitalWrite(i + 1, HIGH);
        delay(PAUSE);
        digitalWrite(i + 1, LOW);
    }
    delay(2500);
    for (int n = 0; n < 16; n++) {
        byte m = mask[n];
        for (int i = 0; i < 7; i++) {
            digitalWrite(i + 1, bitRead(m, i));
        }
        delay(PAUSE);
    }
}
