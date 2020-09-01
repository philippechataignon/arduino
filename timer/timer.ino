const byte Led = 13;            // Pour utiliser la LED du module
#define LedToggle digitalWrite (Led, !digitalRead(Led))

void setup()
{
    pinMode(Led, OUTPUT);
    bitClear(TCCR2A, WGM20);    // WGM20 = 0
    bitClear(TCCR2A, WGM21);    // WGM21 = 0 
    TCCR2B = 0 b00000110;       // Clock / 256 soit 16 micro-s et WGM22 = 0
    TIFR2 = 0 b00000001;        // TOV2
    TCNT2 = 256 - 250;          // Chargement du timer à  6
}

byte varCompteur = 0;           // La variable compteur

void loop()
{
    if (bitRead(TIFR2, 0) == 1) {   // Flag TOV2 mis Ã  1 ?
        TCNT2 = 256 - 250;      // Rechargement du timer 6
        bitSet(TIFR2, TOV2);    // RaZ flag TOV2 (voir texte)
        if (varCompteur++ > 125) {  // Incr et a atteint 125 ?
            varCompteur = 0;    // On recommence un nouveau cycle
            LedToggle;          // Inversion de la LED
        }
    }
}
