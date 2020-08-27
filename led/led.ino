#define LEDPIN 13

#define PAUSEON 500
#define PAUSEOFF 500

void setup() { 
    pinMode(LEDPIN, OUTPUT);
} 

void loop() { 
    digitalWrite(LEDPIN, LOW);
    delay(PAUSEOFF);
    digitalWrite(LEDPIN, HIGH);
    delay(PAUSEON);
}
