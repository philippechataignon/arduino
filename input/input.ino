#define LEDPIN 13
#define BUTPIN 2

#define PAUSEON 500
#define PAUSEOFF 500

void setup()
{
    Serial.begin(9600);
    pinMode(LEDPIN, OUTPUT);
    pinMode(BUTPIN, INPUT_PULLUP);
}

void loop()
{
    while (digitalRead(BUTPIN) == HIGH)
        delay(10);
    Serial.println("on");
    digitalWrite(LEDPIN, HIGH);
    while (digitalRead(BUTPIN) == LOW)
        delay(10);
    Serial.println("off");
    digitalWrite(LEDPIN, LOW);
}
