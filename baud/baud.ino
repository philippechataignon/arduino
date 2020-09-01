void setup()
{
    Serial.begin(117500);
}

void loop()
{
    while (Serial.available()) {
        Serial.read();
    }
}
