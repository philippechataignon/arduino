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
    for (int i = 1; i < 14; i++) {
        pinMode(i, OUTPUT);
    }
    for (int i = 10; i < 14; i++) {
        digitalWrite(i, HIGH);
    }
}

void loop()
{
    // first number
    for (int n = 0; n <= 0xFFFF; n++) {
        digitalWrite(10, LOW);
        byte m = mask[n & 0x000f];
        // send active segments
        for (int i = 0; i < 7; i++) {
            digitalWrite(i + 1, bitRead(m, i));
        }
        delay(1);
        digitalWrite(10, HIGH);
        m = mask[(n >> 4) & 0x000f];
        digitalWrite(11, LOW);
        for (int i = 0; i < 7; i++) {
            digitalWrite(i + 1, bitRead(m, i));
        }
        delay(1);
        digitalWrite(11, HIGH);
        m = mask[(n >> 8) & 0x000f];
        digitalWrite(12, LOW);
        for (int i = 0; i < 7; i++) {
            digitalWrite(i + 1, bitRead(m, i));
        }
        delay(1);
        digitalWrite(12, HIGH);
        m = mask[(n >> 12) & 0x000f];
        digitalWrite(13, LOW);
        for (int i = 0; i < 7; i++) {
            digitalWrite(i + 1, bitRead(m, i));
        }
        delay(1);
        digitalWrite(13, HIGH);
    }
}
