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
void envoi_ordre(int dataPin, int clockPin, boolean sens, char donnee)
{
    //on va parcourir chaque bit de l'octet
    for (int i = 0; i < 8; i++) {
        //on met l'horloge à l'état bas
        digitalWrite(clockPin, LOW);
        //on met le bit de donnée courante en place
        if (sens) {
            digitalWrite(dataPin, donnee & 0x01 << i);
        } else {
            digitalWrite(dataPin, donnee & 0x80 >> i);
        }
        //enfin on remet l'horloge à l'état haut pour faire prendre en compte cette dernière
        digitalWrite(clockPin, HIGH);
    }
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
    // un chenillard, c'est une led allumée à la fois et qui se déplace
    // il faut allumer la première, puis l'éteindre puis la deuxième etc.
    // l'octet représentent les 8 leds, il faut donc avoir 
    // 0000 0001
    // 0000 0010
    // 0000 0100 et ainsi de suite
    // on remarque que les valeurs décimales de l'octet sont 1 puis 2 puis 4
    // pratique, il suffit de faire x2 pour décaler vers la gauche

    // départ i=1 pour la première led et x 2 à chaque boucle
    // lorsque i aura atteint 128, le dernier x2 donnera 256 soit 1 0000 0000
    // on voit que le 1 ne fait plus parti de l'octet donc i = 0, d'ou la condition de sortie
    // et ensuite on reprend
    for (unsigned char i = 1; i > 0; i = i * 2) {
        Serial.println(i);
        //On active le verrou le temps de transférer les données
        digitalWrite(LOCK, LOW);
        //on envoi toutes les données grâce à notre belle fonction
        envoi_ordre(DATA, CLOCK, 1, i);
        //et enfin on relâche le verrou
        digitalWrite(LOCK, HIGH);
        //une petite pause pour constater l'affichage 
        delay(250);
    }
}
