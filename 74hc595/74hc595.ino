/*-- pin out du 74HC595
         1___o___16    
PO-Q1 <-  |     | <- Vcc (+5v)
PO-Q2 <-  |  C  | -> PO-Q0
PO-Q3 <-  |  D  | <- DS entrée série -> PIN 11
PO-Q4 <-  |  4  | <- OE Output Enable (active LOW) maintenir à GND
PO-Q5 <-  |  0  | <- ST-CP verrouillage des sorties -> PIN 12
PO-Q6 <-  |  2  | <- SH-CP entrée de l'horloge de décalage -> PIN 13
PO-Q7 <-  |  1  | <- MR Master Reset (active LOW) maintenir à +Vcc
  GND ->  |_____| -> Q7'
         8       9
PO-Q0 à Q7 sont les sorties parallèles
Q7' est l'équivalent de Q7 pour connecter à un autre registre
DS est l'entrée série 
*/

//Broche connectée au ST_CP du 74HC595
#define LOCK 12
//Broche connectée au SH_CP du 74HC595
#define CLOCK 13
//Broche connectée au DS du 74HC595
#define DATA 11

// cette fonction permet de "pousser" un octet dans le registre à décalage
// la fonction met un front bas sur l'horloge, présente la valeur au registre
// (soit la valeur du bit de l'octet que la fonction est en train de lire
// puis met un front haut pour valider la valeur
void send_byte(byte donnee)
{
    //On active le verrou le temps de transférer les données
    digitalWrite(LOCK, LOW);
    Serial.println(donnee);
    for (byte i = 0; i < 8; i++) {
        //on met l'horloge à l'état bas
        digitalWrite(CLOCK, LOW);
        //on met le bit courant en place
        digitalWrite(DATA, donnee & (1u << i));
        //enfin on remet l'horloge à l'état haut pour faire prendre en compte cette dernière
        digitalWrite(CLOCK, HIGH);
    }
    //et enfin on relâche le verrou
    digitalWrite(LOCK, HIGH);
}

void setup()
{
    //On met les broches en sortie
    pinMode(LOCK, OUTPUT);
    pinMode(CLOCK, OUTPUT);
    pinMode(DATA, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    byte val = 0;
    for (int i = 0; i < 8; i++) {
        val = 1u << i;
        send_byte(val);
        delay(250);
    }
    for (int i = 6; i >= 0; i--) {
        Serial.print(">i:");
        Serial.println(i);
        //met bit i à 1
        val |= (1u << i);
        send_byte(val);
        delay(250);
    }
    for (int i = 0; i < 7; i++) {
        Serial.print(">i:");
        Serial.println(i);
        //met bit i à 0
        val &= (~(1u << i));
        send_byte(val);
        delay(250);
    }
    for (int i = 7; i > 0; i--) {
        val = 1u << i;
        send_byte(val);
        delay(250);
    }
}
