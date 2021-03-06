/*-- pin out du 74HC595
         1___o___16    
   Q1 <-  |     | <- Vcc (+5v)
   Q2 <-  |  C  | -> Q0
   Q3 <-  |  D  | <- DS entrée série -> PIN 11
   Q4 <-  |  4  | <- OE Output Enable (active LOW) maintenir à GND
   Q5 <-  |  0  | <- RCLK  = LOCK    -> PIN 12
   Q6 <-  |  2  | <- SRCLK = CLOCK   -> PIN 13
   Q7 <-  |  1  | <- MR Master Reset (active LOW) maintenir à +Vcc
  GND ->  |_____| -> Q7' -> DS si plusieurs 74HC595 (*RCLK à connecter)
         8       9
Q0 à Q7 sont les sorties; DS est l'entrée série 
RCLK (Register Clock): doit être mis en High pour valider les nouveaux shifts register.
SRCLK (Serial Clock): déplace le registre lorsqu’il est mis à 1 (High).
*/

//Broche connectée au RCLK du 74HC595
#define LOCK 12
//Broche connectée au SRCLK du 74HC595
#define CLOCK 13
//Broche connectée au DS du 74HC595
#define DATA 11

//How many shift registers
#define NUM_SR 1
#define NUM_REG 8 * NUM_SR

void send_value(boolean boolarray[])
{
    // lock for data transfer
    digitalWrite(LOCK, LOW);
    for (byte i = 0; i < NUM_REG; i++) {
        // clock LOW
        digitalWrite(CLOCK, LOW);
        // write bit
        digitalWrite(DATA, boolarray[i]);
        // clock HIGH for validation
        digitalWrite(CLOCK, HIGH);
    }
    // unlock
    digitalWrite(LOCK, HIGH);
}

boolean val[NUM_REG] = { 0 };

void setup()
{
    pinMode(LOCK, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(DATA, OUTPUT);
    // clear 74hc595
    send_value(val);
}

void loop()
{
    int d = 500;
    int i = 0;
    for (i = 0; i < 8; i++) {
        val[i] = 1;
        send_value(val);
        val[i] = 0;
        delay(d);
    }
    for (i = 6; i > 0; i--) {
        val[i] = 1;
        send_value(val);
        val[i] = 0;
        delay(d);
    }
    for (i = 0; i < 7; i++) {
        val[i] = 1;
        send_value(val);
        val[i] = 0;
        delay(d);
    }
    for (i = 7; i >= 0; i--) {
        //met bit i à 1
        val[i] = 1;
        send_value(val);
        delay(d);
    }
    for (i = 7; i > 0; i--) {
        //met bit i à 0
        val[i] = 0;
        send_value(val);
        delay(d);
    }
}
