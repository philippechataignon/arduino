void intr()
{
    digitalWrite(13, HIGH);
    delay(1000);
}

void setup()
{
    pinMode(13, OUTPUT);
    attachInterrupt(0, intr, CHANGE);
}

void loop()
{
    digitalWrite(13, LOW);
}

