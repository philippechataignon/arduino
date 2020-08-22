#define LEDPIN 12

#define PAUSEON 500
#define PAUSEOFF 500

void setup() { 
    pinMode(LEDPIN, OUTPUT);
} 

void loop() { 
    // digitalWrite(LEDPIN, digitalRead(BUTPIN));
    digitalWrite(LEDPIN, LOW);
    delay(2500);
    digitalWrite(LEDPIN, HIGH);
    delay(500);
}
