#define LEDPIN 12
#define BUTPIN 10

#define PAUSEON 500
#define PAUSEOFF 500

void setup() { 
    Serial.begin(9600);
    pinMode(LEDPIN, OUTPUT);
    pinMode(BUTPIN, INPUT_PULLUP);
} 

void loop() { 
    while (digitalRead(BUTPIN) == HIGH)
        delay(50);
    Serial.println("on");
    digitalWrite(LEDPIN, HIGH);
    delay(2000);
    while (digitalRead(BUTPIN) == LOW)
        delay(50);
    Serial.println("off");
    digitalWrite(LEDPIN, LOW);
}
