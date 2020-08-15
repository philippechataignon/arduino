#define LEDPIN 12
#define BUTPIN 10

#define PAUSEON 500
#define PAUSEOFF 500

void setup() { 
    pinMode(LEDPIN, OUTPUT);
    pinMode(BUTPIN, INPUT);
} 

void loop() { 
    // digitalWrite(LEDPIN, digitalRead(BUTPIN));
    digitalWrite(LEDPIN, LOW);
    delay(500);
    digitalWrite(LEDPIN, HIGH);
    delay(500);
}
