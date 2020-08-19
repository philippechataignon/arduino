class Button {
    int btnPin;      // the number of the LED pin
    unsigned long OnTime;     // milliseconds of on-time
    unsigned long IncrTime;
    unsigned long Offset;
     
    // These maintain the current state
    int btnState;             		// btnState used to set the LED
    int btnState0;             		// btnState used to set the LED
    int btnLevel;             		// btnLevel 0-255
    unsigned long previousMillis;  	// will store last time LED was updated
    unsigned long previousMillisLevel;  	// will store last time LED was updated
     
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Button(int pin, long on, long incr = 10, long offset = 0) {
    btnPin = pin;
    pinMode(btnPin, INPUT_PULLUP);     
    	  
    OnTime = on;
    IncrTime = incr;
    Offset = offset;
    btnState = LOW; 
    btnLevel = 0; 
    previousMillis = 0;
  }
     
  void Update() {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
    if(digitalRead(btnPin) == LOW) {
        btnState0 = HIGH; 
        previousMillis = currentMillis;  // Remember the time
    } 

    if(btnState0 == HIGH && (currentMillis - previousMillis) >= Offset) {
        btnState = HIGH; 
        btnState0 = LOW;
        previousMillis = currentMillis;  // Remember the time
    } 
         
    if(btnState == HIGH && (currentMillis - previousMillis) >= OnTime) {
        btnState = LOW; 
        previousMillis = currentMillis;  // Remember the time
    } 

    if(btnState == HIGH && (currentMillis - previousMillisLevel) >= IncrTime && btnLevel < 255) {
        btnLevel += 1;
        previousMillisLevel = currentMillis;  // Remember the time
    }
    if(btnState == LOW && (currentMillis - previousMillisLevel) >= IncrTime && btnLevel > 0) {
        btnLevel -= 1;
        previousMillisLevel = currentMillis;  // Remember the time
    }
  }

  int getState() {
    return btnState;
  }
  int getLevel() {
    return btnLevel;
  }
};


class Flasher {
    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    unsigned long OnTime;     // milliseconds of on-time
    unsigned long OffTime;    // milliseconds of off-time
     
    // These maintain the current state
    int ledState;             		// ledState used to set the LED
    unsigned long previousMillis;  	// will store last time LED was updated
     
  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off) {
    ledPin = pin;
    pinMode(ledPin, OUTPUT);     
    	  
    OnTime = on;
    OffTime = off;
    	
    ledState = LOW; 
    previousMillis = 0;
  }
     
  void Update() {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
         
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime)) {
        ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    } else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime)) {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }
};
     
Flasher led1(13, 350, 350);
// Flasher led2(12, 100, 400);
Button btn1(10, 2000);
Button btn2(10, 2000, 10, 1000);
Button btn3(10, 2000, 10, 2000);
     
void setup() {
    pinMode(3, OUTPUT);     
    pinMode(5, OUTPUT);     
    pinMode(6, OUTPUT);     
}
     
void loop() {
    led1.Update();
    analogWrite(3, btn1.getLevel());
    analogWrite(5, btn2.getLevel());
    analogWrite(6, btn3.getLevel());
//    led2.Update();
    btn1.Update();
    btn2.Update();
    btn3.Update();
}
