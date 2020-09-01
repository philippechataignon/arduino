volatile long deb_time = 0;
volatile long cur_time = 0;

void intr()
{
    cur_time = millis();
    if ((cur_time - deb_time) > 200) {
        Serial.println(">Intr");
        digitalWrite(13, !digitalRead(13));
    }
    deb_time = millis();
}

void setup()
{
    pinMode(13, OUTPUT);
    pinMode(2, INPUT);          // INT0 = pin 2 = PD2
    attachInterrupt(0, intr, FALLING);
    Serial.begin(9600);
}

void loop()
{
    Serial.println("Bip");
    delay(1000);
}
