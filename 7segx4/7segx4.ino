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
    for (int k = 0; k < 13; k++) {
        for (int d = 0; d < 128; d++) {
            int n = k;
            for (int i = 1; i < 8; i++) {
                digitalWrite(i, LOW);
            }
            digitalWrite(10, LOW);
            byte m = mask[n];
            for (int i = 0; i < 7; i++) {
                digitalWrite(i + 1, bitRead(m, i));
            }
            delay(1);
            digitalWrite(10, HIGH);
            digitalWrite(11, LOW);
            n++;
            m = mask[n];
            for (int i = 0; i < 7; i++) {
                digitalWrite(i + 1, bitRead(m, i));
            }
            delay(1);
            digitalWrite(11, HIGH);
            digitalWrite(12, LOW);
            n++;
            m = mask[n];
            for (int i = 0; i < 7; i++) {
                digitalWrite(i + 1, bitRead(m, i));
            }
            delay(1);
            digitalWrite(12, HIGH);
            digitalWrite(13, LOW);
            n++;
            m = mask[n];
            for (int i = 0; i < 7; i++) {
                digitalWrite(i + 1, bitRead(m, i));
            }
            delay(1);
            digitalWrite(13, HIGH);
        }
    }
}
